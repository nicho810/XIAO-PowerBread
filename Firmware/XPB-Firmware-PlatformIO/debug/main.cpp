/**
 * XPB Binary Protocol v1.1 -- RP2040 硬件模拟器
 *
 * 跑在 XIAO RP2040 上，通过 USB-CDC Serial 输出协议帧流。
 * 零外部库依赖，Arduino setup/loop 范式。
 *
 * 构建:  pio run -e simulator
 * 烧录:  pio run -e simulator -t upload
 * 监控:  pio device monitor -b 115200
 *
 * [INPUT]: 依赖 Arduino 核心 (Serial, millis, random, pinMode, digitalWrite)
 * [OUTPUT]: USB-CDC 输出 XPB Binary Protocol v1.1 帧流
 * [POS]: debug/ 的唯一成员，独立协议模拟器，不依赖主固件任何模块
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include <Arduino.h>
#include <string.h>
#include <math.h>

/* ── 协议常量 ─────────────────────────────────────────── */

static constexpr uint8_t SYNC_0            = 0xAA;
static constexpr uint8_t SYNC_1            = 0x55;
static constexpr uint8_t TYPE_REALTIME     = 0x01;
static constexpr uint8_t TYPE_START        = 0xC0;
static constexpr uint8_t TYPE_CONFIG_ACK   = 0xC1;
static constexpr uint8_t TYPE_STOP         = 0xC2;
static constexpr uint8_t TYPE_CONFIG       = 0xFE;
static constexpr uint8_t PROTOCOL_VER      = 0x01;

static constexpr int     CONFIG_RETRY_MAX  = 3;
static constexpr uint32_t CONFIG_TIMEOUT_MS = 500;

/* ── 设备参数 ─────────────────────────────────────────── */

static constexpr float   SHUNT_R_A      = 0.050f;   // 50 mΩ
static constexpr float   SHUNT_R_B      = 0.050f;
static constexpr uint32_t DATA_INTERVAL_MS = 10;     // 100 Hz
static constexpr int     DATA_HZ        = 100;

/* ── RGB LED (XIAO RP2040: PIN_LED_R/G/B 由 board variant 定义, 低电平有效) */

static void led_off()  { digitalWrite(PIN_LED_R, HIGH); digitalWrite(PIN_LED_G, HIGH); digitalWrite(PIN_LED_B, HIGH); }
static void led_red()  { led_off(); digitalWrite(PIN_LED_R, LOW); }
static void led_blue() { led_off(); digitalWrite(PIN_LED_B, LOW); }
static void led_green(){ led_off(); digitalWrite(PIN_LED_G, LOW); }

/* ── CRC-8/MAXIM ──────────────────────────────────────── */

static uint8_t crc8_maxim(const uint8_t *data, size_t len) {
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

static size_t build_frame(uint8_t *buf, uint8_t type,
                          const uint8_t *payload, uint8_t payload_len) {
    buf[0] = SYNC_0;
    buf[1] = SYNC_1;
    buf[2] = type;
    buf[3] = payload_len;
    if (payload_len > 0) {
        memcpy(buf + 4, payload, payload_len);
    }
    uint8_t crc = crc8_maxim(buf + 2, 2 + payload_len);
    buf[4 + payload_len] = crc;
    return 5 + payload_len;
}

/* ── 帧序列化工具 ─────────────────────────────────────── */

static void pack_f32(uint8_t *dst, float val) {
    memcpy(dst, &val, 4);
}

static void pack_u32(uint8_t *dst, uint32_t val) {
    memcpy(dst, &val, 4);
}

/* ── 帧解析器 ─────────────────────────────────────────── */

struct ParsedFrame {
    uint8_t type;
    uint8_t len;
    uint8_t payload[32];
};

class FrameParser {
public:
    void reset() { state_ = S_SYNC_0; }

    /* 逐字节喂入，返回 true 表示一帧完整且 CRC 校验通过 */
    bool feed(uint8_t byte, ParsedFrame &out) {
        switch (state_) {
        case S_SYNC_0:
            if (byte == SYNC_0) state_ = S_SYNC_1;
            return false;

        case S_SYNC_1:
            if (byte == SYNC_1) { state_ = S_TYPE; }
            else if (byte != SYNC_0) { state_ = S_SYNC_0; }
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
            remaining_ = frame_len_ + 1;  // payload + crc
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
                uint8_t crc_calc = crc8_maxim(buf_, idx_ - 1);
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

private:
    enum State { S_SYNC_0, S_SYNC_1, S_TYPE, S_LEN, S_BODY };
    State    state_      = S_SYNC_0;
    uint8_t  frame_type_ = 0;
    uint8_t  frame_len_  = 0;
    uint8_t  buf_[64]    = {};
    size_t   idx_        = 0;
    size_t   remaining_  = 0;
};

/* ── CONFIG 帧构建 ────────────────────────────────────── */

static size_t build_config_frame(uint8_t *buf) {
    uint8_t payload[9];
    pack_f32(payload + 0, SHUNT_R_A);
    pack_f32(payload + 4, SHUNT_R_B);
    payload[8] = PROTOCOL_VER;
    return build_frame(buf, TYPE_CONFIG, payload, 9);
}

/* ── 高斯噪声 (Box-Muller) ───────────────────────────── */

static float gaussian(float sigma) {
    float u1 = (random(1, 10000) + 0.5f) / 10000.0f;
    float u2 = (random(1, 10000) + 0.5f) / 10000.0f;
    return sigma * sqrtf(-2.0f * logf(u1)) * cosf(2.0f * (float)M_PI * u2);
}

/* ── 数据生成 ─────────────────────────────────────────── */

static void generate_sensor_data(uint8_t *payload) {
    uint32_t ts = millis();
    float t = ts * 0.001f;

    /* Channel A: 5V + 正弦扰动 */
    float busV_a    = 5.0f + gaussian(0.020f);
    float current_a = 0.050f + 0.010f * sinf(2.0f * (float)M_PI * 0.5f * t);
    float shuntV_a  = current_a * SHUNT_R_A;

    /* Channel B: 3.3V + 偶发尖峰 */
    float busV_b    = 3.3f + gaussian(0.010f);
    float current_b = 0.030f;
    if (random(100) < 2) {
        current_b = 0.080f + (random(0, 700) * 0.0001f);
    }
    float shuntV_b = current_b * SHUNT_R_B;

    pack_f32(payload + 0,  busV_a);
    pack_f32(payload + 4,  shuntV_a);
    pack_f32(payload + 8,  busV_b);
    pack_f32(payload + 12, shuntV_b);
    pack_u32(payload + 16, ts);
}

/* ── 状态机 ───────────────────────────────────────────── */

enum class SimState { IDLE, WAIT_ACK, STREAMING };

static SimState      g_state;
static FrameParser   g_parser;
static uint32_t      g_last_send_ms;
static uint32_t      g_ack_deadline;
static int           g_retry_count;

/* ── setup ────────────────────────────────────────────── */

void setup() {
    /* LED 初始化 */
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    led_red();

    Serial.begin(115200);

    /* 随机种子: 浮空 ADC */
    randomSeed(analogRead(A0) ^ (micros() << 8));

    g_state       = SimState::IDLE;
    g_last_send_ms = 0;
    g_ack_deadline = 0;
    g_retry_count  = 0;
    g_parser.reset();
}

/* ── 非阻塞解析所有可用字节 ───────────────────────────── */

static bool drain_serial(ParsedFrame &frame) {
    while (Serial.available()) {
        uint8_t byte = Serial.read();
        if (g_parser.feed(byte, frame)) return true;
    }
    return false;
}

/* ── loop ─────────────────────────────────────────────── */

void loop() {
    ParsedFrame frame;

    switch (g_state) {

    /* ── IDLE: 等待 START ──────────────────────────────── */
    case SimState::IDLE:
        if (drain_serial(frame) && frame.type == TYPE_START) {
            /* 收到 START，发第一个 CONFIG */
            uint8_t cfgbuf[14];
            size_t len = build_config_frame(cfgbuf);
            Serial.write(cfgbuf, len);

            g_retry_count  = 1;
            g_ack_deadline = millis() + CONFIG_TIMEOUT_MS;
            g_parser.reset();
            g_state = SimState::WAIT_ACK;
            led_blue();
        }
        break;

    /* ── WAIT_ACK: 等 CONFIG_ACK，超时重发 ─────────────── */
    case SimState::WAIT_ACK:
        if (drain_serial(frame)) {
            if (frame.type == TYPE_CONFIG_ACK) {
                /* 握手完成，进入流模式 */
                g_last_send_ms = millis();
                g_parser.reset();
                g_state = SimState::STREAMING;
                led_green();
                break;
            }
            /* 非 ACK 帧，忽略继续等 */
        }

        /* 超时检查 */
        if (millis() >= g_ack_deadline) {
            if (g_retry_count >= CONFIG_RETRY_MAX) {
                /* 重试耗尽，回 IDLE */
                g_parser.reset();
                g_state = SimState::IDLE;
                led_red();
            } else {
                /* 重发 CONFIG */
                uint8_t cfgbuf[14];
                size_t len = build_config_frame(cfgbuf);
                Serial.write(cfgbuf, len);

                g_retry_count++;
                g_ack_deadline = millis() + CONFIG_TIMEOUT_MS;
                g_parser.reset();
            }
        }
        break;

    /* ── STREAMING: 100Hz 发 DATA，非阻塞检查 STOP ────── */
    case SimState::STREAMING:
        /* 检查控制帧 */
        if (drain_serial(frame)) {
            if (frame.type == TYPE_STOP) {
                g_parser.reset();
                g_state = SimState::IDLE;
                led_red();
                break;
            }
            if (frame.type == TYPE_START) {
                /* 重新握手 */
                uint8_t cfgbuf[14];
                size_t len = build_config_frame(cfgbuf);
                Serial.write(cfgbuf, len);

                g_retry_count  = 1;
                g_ack_deadline = millis() + CONFIG_TIMEOUT_MS;
                g_parser.reset();
                g_state = SimState::WAIT_ACK;
                led_blue();
                break;
            }
        }

        /* 定时发送数据帧 */
        if (millis() - g_last_send_ms >= DATA_INTERVAL_MS) {
            g_last_send_ms += DATA_INTERVAL_MS;

            uint8_t payload[20];
            generate_sensor_data(payload);

            uint8_t df[25];
            size_t dlen = build_frame(df, TYPE_REALTIME, payload, 20);
            Serial.write(df, dlen);
        }
        break;
    }
}
