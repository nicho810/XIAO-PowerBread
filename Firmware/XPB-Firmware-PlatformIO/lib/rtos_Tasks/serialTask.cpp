/**
 * [INPUT]: serialTask.h, sysConfig.h, xpb_protocol.h, sensorDataMutex/latestSensorData (extern)
 * [OUTPUT]: serialPrintTask() 双模串口任务实现
 * [POS]: rtos_Tasks 的串口任务，IDLE 时文本调试，收到 START 自动切二进制协议流 (100Hz)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include "serialTask.h"
#include "sysConfig.h"
#include "xpb_protocol.h"

extern ConfigMode configMode;
extern SysConfig sysConfig;

extern SemaphoreHandle_t sensorDataMutex;
extern DualChannelData latestSensorData;

/* ── 文本模式输出 ─────────────────────────────────────── */

static void print_text(const DualChannelData &data, uint8_t mode)
{
    char buffer[150];
    if (mode == 0)
    {
        snprintf(buffer, sizeof(buffer),
                 "A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
                 data.channel0.busVoltage, data.channel0.shuntVoltage * 1000,
                 data.channel0.busCurrent, data.channel0.busPower,
                 data.channel1.busVoltage, data.channel1.shuntVoltage * 1000,
                 data.channel1.busCurrent, data.channel1.busPower);
    }
    else
    {
        snprintf(buffer, sizeof(buffer),
                 "V0:%.2f,I0:%.2f,P0:%.2f,V1:%.2f,I1:%.2f,P1:%.2f\n",
                 data.channel0.busVoltage, data.channel0.busCurrent,
                 data.channel0.busPower, data.channel1.busVoltage,
                 data.channel1.busCurrent, data.channel1.busPower);
    }
    Serial.print(buffer);
    Serial.flush();
}

/* ── 二进制数据帧发送 ─────────────────────────────────── */

static void send_realtime_frame(void)
{
    if (xSemaphoreTake(sensorDataMutex, pdMS_TO_TICKS(2)) == pdTRUE)
    {
        DualChannelData data = latestSensorData;
        xSemaphoreGive(sensorDataMutex);

        uint8_t buf[25];
        size_t len = xpb_build_realtime_frame(buf,
            data.channel0.busVoltage,
            data.channel0.shuntVoltage / 1000.0f,   // mV → V
            data.channel1.busVoltage,
            data.channel1.shuntVoltage / 1000.0f,   // mV → V
            millis());
        Serial.write(buf, len);
    }
}

/* ── 执行 session 动作 ────────────────────────────────── */

static void execute_action(const XpbSessionAction &action)
{
    if (action.tx_data && action.tx_len > 0)
    {
        Serial.write(action.tx_data, action.tx_len);
    }
    if (action.send_data)
    {
        send_realtime_frame();
    }
}

/* ── 双模串口任务 (5ms 基础循环) ──────────────────────── */

void serialPrintTask(void *pvParameters)
{
    (void)pvParameters;

    /* 读取配置 (启动时一次) */
    const bool serialEnabled = sysConfig.cfg_data.serial_enable;
    const uint8_t serialMode = sysConfig.cfg_data.serial_mode;

    static const uint32_t textIntervals[] = { 1000, 500, 100, 50, 10 };
    uint8_t idx = min(sysConfig.cfg_data.serial_printInterval, (uint8_t)4);
    const uint32_t textInterval = textIntervals[idx];

    /* 协议引擎初始化 */
    XpbFrameParser parser;
    XpbSession session;

    float shuntR_a = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;   // mΩ → Ω
    float shuntR_b = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;
    session.set_shunt_config(shuntR_a, shuntR_b);

    uint32_t lastTextPrint = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5));
        uint32_t now = millis();

        /* ── Phase 1: 非阻塞排空 RX → parser → session ── */
        if (Serial)
        {
            XpbParsedFrame frame;
            while (Serial.available())
            {
                uint8_t byte = Serial.read();
                if (parser.feed(byte, frame))
                {
                    XpbSessionAction action = session.on_frame(frame, now);
                    execute_action(action);
                    if (action.reset_parser) parser.reset();
                }
            }
        }

        /* ── Phase 2: session tick (超时重发 / 数据节拍) ── */
        {
            XpbSessionAction action = session.on_tick(now);
            execute_action(action);
            if (action.reset_parser) parser.reset();
        }

        /* ── Phase 3: 模式分发 ────────────────────────── */
        if (configMode.configState.isActive) continue;

        if (session.state() == XpbSession::IDLE && serialEnabled && Serial)
        {
            if (now - lastTextPrint >= textInterval)
            {
                lastTextPrint = now;
                if (xSemaphoreTake(sensorDataMutex, pdMS_TO_TICKS(100)) == pdTRUE)
                {
                    DualChannelData data = latestSensorData;
                    xSemaphoreGive(sensorDataMutex);
                    print_text(data, serialMode);
                }
            }
        }
    }
}
