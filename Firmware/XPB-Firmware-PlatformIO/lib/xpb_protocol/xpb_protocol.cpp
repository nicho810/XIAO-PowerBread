/**
 * [INPUT]: xpb_protocol.h, <string.h>
 * [OUTPUT]: CRC 计算 + 帧构建 + 帧解析 + 会话状态机实现
 * [POS]: xpb_protocol 模块的唯一实现文件
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include "xpb_protocol.h"
#include <string.h>

/* ── 序列化工具 ───────────────────────────────────────── */

static void pack_f32(uint8_t *dst, float val) {
    memcpy(dst, &val, 4);
}

static void pack_u32(uint8_t *dst, uint32_t val) {
    memcpy(dst, &val, 4);
}

/* ── CRC-8/MAXIM ──────────────────────────────────────── */

uint8_t xpb_crc8_maxim(const uint8_t *data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
        }
    }
    return crc;
}

/* ── 帧构建 ───────────────────────────────────────────── */

size_t xpb_build_frame(uint8_t *buf, uint8_t type,
                        const uint8_t *payload, uint8_t payload_len) {
    buf[0] = XPB_SYNC_0;
    buf[1] = XPB_SYNC_1;
    buf[2] = type;
    buf[3] = payload_len;
    if (payload_len > 0) {
        memcpy(buf + 4, payload, payload_len);
    }
    buf[4 + payload_len] = xpb_crc8_maxim(buf + 2, 2 + payload_len);
    return 5 + payload_len;
}

size_t xpb_build_config_frame(uint8_t *buf, float shunt_r_a, float shunt_r_b) {
    uint8_t payload[9];
    pack_f32(payload + 0, shunt_r_a);
    pack_f32(payload + 4, shunt_r_b);
    payload[8] = XPB_PROTOCOL_VER;
    return xpb_build_frame(buf, XPB_TYPE_CONFIG, payload, 9);
}

size_t xpb_build_realtime_frame(uint8_t *buf,
                                 float busV_a, float shuntV_a,
                                 float busV_b, float shuntV_b,
                                 uint32_t timestamp) {
    uint8_t payload[20];
    pack_f32(payload + 0,  busV_a);
    pack_f32(payload + 4,  shuntV_a);
    pack_f32(payload + 8,  busV_b);
    pack_f32(payload + 12, shuntV_b);
    pack_u32(payload + 16, timestamp);
    return xpb_build_frame(buf, XPB_TYPE_REALTIME, payload, 20);
}

/* ── 帧解析器 ─────────────────────────────────────────── */

bool XpbFrameParser::feed(uint8_t byte, XpbParsedFrame &out) {
    switch (state_) {
    case S_SYNC_0:
        if (byte == XPB_SYNC_0) state_ = S_SYNC_1;
        return false;

    case S_SYNC_1:
        if (byte == XPB_SYNC_1) { state_ = S_TYPE; }
        else if (byte != XPB_SYNC_0) { state_ = S_SYNC_0; }
        return false;

    case S_TYPE:
        frame_type_ = byte;
        buf_[0] = byte;
        state_ = S_LEN;
        return false;

    case S_LEN:
        frame_len_ = byte;
        buf_[1] = byte;
        idx_ = 2;
        remaining_ = frame_len_ + 1;   // payload + crc
        if (remaining_ > sizeof(buf_) - 2) {
            state_ = S_SYNC_0;
            return false;
        }
        state_ = (remaining_ > 0) ? S_BODY : S_SYNC_0;
        return false;

    case S_BODY:
        buf_[idx_++] = byte;
        if (--remaining_ == 0) {
            state_ = S_SYNC_0;
            uint8_t crc_recv = buf_[idx_ - 1];
            uint8_t crc_calc = xpb_crc8_maxim(buf_, idx_ - 1);
            if (crc_recv == crc_calc) {
                out.type = frame_type_;
                out.len  = frame_len_;
                if (frame_len_ > 0) {
                    memcpy(out.payload, buf_ + 2, frame_len_);
                }
                return true;
            }
        }
        return false;
    }
    return false;
}

/* ── 会话状态机 ───────────────────────────────────────── */

void XpbSession::set_shunt_config(float shunt_r_a, float shunt_r_b) {
    config_len_ = xpb_build_config_frame(config_buf_, shunt_r_a, shunt_r_b);
}

XpbSessionAction XpbSession::begin_handshake(uint32_t now_ms) {
    retry_count_  = 1;
    ack_deadline_ = now_ms + XPB_CONFIG_TIMEOUT_MS;
    state_        = WAIT_ACK;
    return { config_buf_, config_len_, false, true };
}

XpbSessionAction XpbSession::on_frame(const XpbParsedFrame &frame, uint32_t now_ms) {
    switch (state_) {
    case IDLE:
        if (frame.type == XPB_TYPE_START) {
            return begin_handshake(now_ms);
        }
        break;

    case WAIT_ACK:
        if (frame.type == XPB_TYPE_CONFIG_ACK) {
            last_data_ms_ = now_ms;
            state_        = STREAMING;
            return { nullptr, 0, false, true };
        }
        /* START ignored in WAIT_ACK (协议 3.3 节) */
        break;

    case STREAMING:
        if (frame.type == XPB_TYPE_STOP) {
            state_ = IDLE;
            return { nullptr, 0, false, true };
        }
        if (frame.type == XPB_TYPE_START) {
            return begin_handshake(now_ms);
        }
        break;
    }

    return { nullptr, 0, false, false };
}

XpbSessionAction XpbSession::on_tick(uint32_t now_ms) {
    switch (state_) {
    case WAIT_ACK:
        if (now_ms >= ack_deadline_) {
            if (retry_count_ >= XPB_CONFIG_RETRY_MAX) {
                state_ = IDLE;
                return { nullptr, 0, false, true };
            }
            retry_count_++;
            ack_deadline_ = now_ms + XPB_CONFIG_TIMEOUT_MS;
            return { config_buf_, config_len_, false, true };
        }
        break;

    case STREAMING:
        if (now_ms - last_data_ms_ >= XPB_DATA_INTERVAL_MS) {
            last_data_ms_ += XPB_DATA_INTERVAL_MS;   // 累加式，防长期漂移
            return { nullptr, 0, true, false };
        }
        break;

    default:
        break;
    }

    return { nullptr, 0, false, false };
}
