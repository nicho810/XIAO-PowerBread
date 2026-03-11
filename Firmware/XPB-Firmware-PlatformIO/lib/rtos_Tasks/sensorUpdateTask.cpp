/**
 * [INPUT]: INA3221Sensor (I2C 采集), sensorUpdateTask.h (EVT_* 事件位)
 * [OUTPUT]: latestSensorData + avgS/avgM/avgH/peak (通过 sensorDataMutex 保护)
 * [POS]: 纯数据生产者 (优先级3)，5ms 周期 I2C 采集 + EMA 计算，通过 TaskNotify 驱动 lvglTask
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include "sensorUpdateTask.h"

extern TaskHandle_t xLvglTaskHandle;

/* ================================================================
 *  事件位 — 转发给 lvglTask (与 sensorUpdateTask.h 中 EVT_* 复用)
 * ================================================================ */
#define EVT_SENSOR_READY  (1 << 3)

void sensorUpdateTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(5);
    const float UPDATE_THRESHOLD = 0.005f;

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        /* --- 1. 接收事件 (非阻塞) --- */
        uint32_t events = 0;
        xTaskNotifyWait(0, 0xFFFFFFFF, &events, 0);

        /* --- 2. I2C 采集 (无锁) --- */
        DualChannelData newSensorData = inaSensor.readCurrentSensors();

        /* --- 3. EMA 计算 (无锁，本任务独占写 avg/peak) --- */
        if (current_functionMode == dataMonitorCount)
        {
            for (int ch = 0; ch < 2; ch++)
            {
                float val = (ch == 0) ? newSensorData.channel0.busCurrent
                                      : newSensorData.channel1.busCurrent;
                avgS[ch] = (0.3f * val) + (0.7f * avgS[ch]);
                avgM[ch] = (0.002f * val) + (0.998f * avgM[ch]);
                avgH[ch] = (0.000033f * val) + (0.999967f * avgH[ch]);
                if (val > peak[ch]) peak[ch] = val;
            }
        }

        /* --- 4. 变化检测 + 发布数据 (短锁) --- */
        bool dataChanged = (events & EVT_FORCE_UPDATE) ||
                           (current_functionMode == dataMonitorChart) ||
                           (current_functionMode == dataMonitorCount);

        if (!dataChanged)
        {
            dataChanged = (abs(newSensorData.channel0.busCurrent - latestSensorData.channel0.busCurrent) > UPDATE_THRESHOLD) ||
                          (abs(newSensorData.channel1.busCurrent - latestSensorData.channel1.busCurrent) > UPDATE_THRESHOLD) ||
                          (abs(newSensorData.channel0.busVoltage - latestSensorData.channel0.busVoltage) > UPDATE_THRESHOLD) ||
                          (abs(newSensorData.channel1.busVoltage - latestSensorData.channel1.busVoltage) > UPDATE_THRESHOLD);
        }

        if (dataChanged || (events & (EVT_MODE_CHANGE | EVT_HIGHLIGHT_CHANGE)))
        {
            if (xSemaphoreTake(sensorDataMutex, pdMS_TO_TICKS(2)) == pdTRUE)
            {
                latestSensorData = newSensorData;
                xSemaphoreGive(sensorDataMutex);
            }
        }

        /* --- 5. 转发事件给 lvglTask --- */
        uint32_t fwdBits = events & (EVT_MODE_CHANGE | EVT_HIGHLIGHT_CHANGE | EVT_FORCE_UPDATE);
        if (dataChanged) fwdBits |= EVT_SENSOR_READY;

        if (fwdBits && xLvglTaskHandle)
        {
            xTaskNotify(xLvglTaskHandle, fwdBits, eSetBits);
        }
    }
}
