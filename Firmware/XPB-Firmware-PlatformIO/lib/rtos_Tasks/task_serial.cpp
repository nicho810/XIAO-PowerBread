#include "task_serial.h"

void serialTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastPrintTime = 0;
    const TickType_t xPrintInterval = pdMS_TO_TICKS(1000); // Print every 1 second

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastPrintTime) >= xPrintInterval)
        {
            // Take the semaphore before accessing shared data
            if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
            {
                Serial.print("{\"cSensor\":[");
                for (size_t i = 0; i < cSensorData.size(); i++) {
                    if (i > 0) {
                        Serial.print(",");
                    }
                    Serial.print("{\"ch\":");
                    Serial.print(i);
                    Serial.print(",\"voltage_mV\":");
                    Serial.print(cSensorData[i].busVoltage_mV);
                    Serial.print(",\"current_mA\":");
                    Serial.print(cSensorData[i].current_mA);
                    Serial.print(",\"power_mW\":");
                    Serial.print(cSensorData[i].power_mW);
                    Serial.print("}");
                }
                Serial.println("]}");
                
                // Give back the semaphore
                xSemaphoreGive(xSemaphore);
                
                xLastPrintTime = xCurrentTime;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

        
