#include "task_serial.h"
#include "sysConfig.h"
#include "configMode.h"
// extern ConfigMode configMode;
extern SysConfig sysConfig;

extern SemaphoreHandle_t xSemaphore;
extern DualChannelData latestSensorData;

void serialPrintTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastPrintTime = 0;
    const TickType_t xPrintIntervals[] = {
        pdMS_TO_TICKS(1000), // 0 - 1000ms
        pdMS_TO_TICKS(500),  // 1 - 500ms
        pdMS_TO_TICKS(100),  // 2 - 100ms
        pdMS_TO_TICKS(50),   // 3 - 50ms
        pdMS_TO_TICKS(10)    // 4 - 10ms
    };

    uint8_t intervalIndex = min(sysConfig.cfg_data.serial_printInterval, (uint8_t)4);
    const TickType_t xPrintInterval = xPrintIntervals[intervalIndex];
    const bool serialEnabled = sysConfig.cfg_data.serial_enable;
    const uint8_t serialMode = sysConfig.cfg_data.serial_mode;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastPrintTime) >= xPrintInterval)
        {
            if (!configMode.configState.isActive) // Only run this print when not in config mode
            {
                if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(100)) == pdTRUE)
                {
                    DualChannelData sensorData = latestSensorData;
                    xSemaphoreGive(xSemaphore);

                    if (Serial && serialEnabled)
                    {
                        char buffer[150];
                        if (serialMode == 0)
                        {
                            snprintf(buffer, sizeof(buffer),
                                     "A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
                                     sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000,
                                     sensorData.channel0.busCurrent, sensorData.channel0.busPower,
                                     sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000,
                                     sensorData.channel1.busCurrent, sensorData.channel1.busPower);
                        }
                        else
                        {
                            snprintf(buffer, sizeof(buffer),
                                     "V0:%.2f,I0:%.2f,P0:%.2f,V1:%.2f,I1:%.2f,P1:%.2f\n",
                                     sensorData.channel0.busVoltage, sensorData.channel0.busCurrent,
                                     sensorData.channel0.busPower, sensorData.channel1.busVoltage,
                                     sensorData.channel1.busCurrent, sensorData.channel1.busPower);
                        }
                        Serial.print(buffer);
                        Serial.flush();
                    }
                    xLastPrintTime = xCurrentTime;
                }
            }
        }
    }
}
