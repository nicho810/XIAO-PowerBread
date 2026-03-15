/**
 * XPB Binary Protocol v1.2 — 平台无关协议引擎
 *
 * CRC-8/MAXIM 校验 + 帧构建/解析 + 三阶段握手状态机
 * 零 I/O 依赖：调用者负责串口读写，本模块只产生/消费字节序列
 *
 * [INPUT]: <stdint.h>, <stddef.h>, <string.h> (纯 C++ 标准库)
 * [OUTPUT]: XpbFrameParser, XpbSession, xpb_build_*() 帧构建函数, xpb_crc8_maxim()
 * [POS]: lib/xpb_protocol 的公共接口，被 rtos_Tasks/serialTask 消费
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef XPB_PROTOCOL_H
#define XPB_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

/* ── 协议常量 ─────────────────────────────────────────── */

static constexpr uint8_t  XPB_SYNC_0           = 0xAA;
static constexpr uint8_t  XPB_SYNC_1           = 0x55;

static constexpr uint8_t  XPB_TYPE_REALTIME    = 0x01;
static constexpr uint8_t  XPB_TYPE_START       = 0xC0;
static constexpr uint8_t  XPB_TYPE_CONFIG_ACK  = 0xC1;
static constexpr uint8_t  XPB_TYPE_STOP        = 0xC2;
static constexpr uint8_t  XPB_TYPE_CONFIG      = 0xFE;

static constexpr uint8_t  XPB_PROTOCOL_VER     = 0x01;

static constexpr int      XPB_CONFIG_RETRY_MAX  = 3;
static constexpr uint32_t XPB_CONFIG_TIMEOUT_MS = 500;
static constexpr uint32_t XPB_DATA_INTERVAL_MS  = 10;    // 100 Hz

/* ── 数据结构 ─────────────────────────────────────────── */

struct XpbParsedFrame {
    uint8_t type;
    uint8_t len;
    uint8_t payload[32];
};

struct XpbSessionAction {
    const uint8_t *tx_data;      // 待发送帧 (nullptr = 无)
    size_t         tx_len;       // tx_data 长度
    bool           send_data;    // 调用者构建并发送 REALTIME 帧
    bool           reset_parser; // 调用者重置 FrameParser
};

/* ── 纯函数 ───────────────────────────────────────────── */

uint8_t xpb_crc8_maxim(const uint8_t *data, size_t len);

size_t xpb_build_frame(uint8_t *buf, uint8_t type,
                        const uint8_t *payload, uint8_t payload_len);

size_t xpb_build_config_frame(uint8_t *buf, float shunt_r_a, float shunt_r_b);

size_t xpb_build_realtime_frame(uint8_t *buf,
                                 float busV_a, float shuntV_a,
                                 float busV_b, float shuntV_b,
                                 uint32_t timestamp);

/* ── 帧解析器 (逐字节状态机) ──────────────────────────── */

class XpbFrameParser {
public:
    void reset() { state_ = S_SYNC_0; }

    /* 逐字节喂入，返回 true = 一帧完整且 CRC 校验通过 */
    bool feed(uint8_t byte, XpbParsedFrame &out);

private:
    enum State { S_SYNC_0, S_SYNC_1, S_TYPE, S_LEN, S_BODY };
    State    state_      = S_SYNC_0;
    uint8_t  frame_type_ = 0;
    uint8_t  frame_len_  = 0;
    uint8_t  buf_[64]    = {};
    size_t   idx_        = 0;
    size_t   remaining_  = 0;
};

/* ── 会话状态机 (IDLE → WAIT_ACK → STREAMING) ─────────── */

class XpbSession {
public:
    enum State { IDLE, WAIT_ACK, STREAMING };

    /* 设置 shunt 电阻值 (单位: Ω)，预构建 CONFIG 帧 */
    void set_shunt_config(float shunt_r_a, float shunt_r_b);

    /* 收到完整帧时调用，返回待执行动作 */
    XpbSessionAction on_frame(const XpbParsedFrame &frame, uint32_t now_ms);

    /* 每个循环 tick 调用 (超时重发 / 数据节拍) */
    XpbSessionAction on_tick(uint32_t now_ms);

    State state() const { return state_; }

private:
    /* 共用逻辑: 发 CONFIG → 进入 WAIT_ACK */
    XpbSessionAction begin_handshake(uint32_t now_ms);

    State    state_          = IDLE;
    uint8_t  config_buf_[14] = {};
    size_t   config_len_     = 0;
    int      retry_count_    = 0;
    uint32_t ack_deadline_   = 0;
    uint32_t last_data_ms_   = 0;
};

#endif // XPB_PROTOCOL_H
