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
            // Peek at the latest data from queue (non-blocking)
            SensorDataMessage receivedData;
            if (xQueuePeek(sensorDataQueue, &receivedData, pdMS_TO_TICKS(100)) == pdTRUE) {
                // Process the received data
                Serial.print("{\"cSensor\":[");
                for (size_t i = 0; i < sizeof(receivedData.data)/sizeof(receivedData.data[0]); i++) {
                    if (i > 0) {
                        Serial.print(",");
                    }
                    Serial.print("{\"ch\":");
                    Serial.print(i);
                    Serial.print(",\"voltage_mV\":");
                    Serial.print(receivedData.data[i].busVoltage_mV);
                    Serial.print(",\"current_mA\":");
                    Serial.print(receivedData.data[i].current_mA);
                    Serial.print(",\"power_mW\":");
                    Serial.print(receivedData.data[i].power_mW);
                    Serial.print(",\"timestamp\":");
                    Serial.print(receivedData.timestamp);
                    Serial.print("}");
                }
                Serial.println("]}");
                
                xLastPrintTime = xCurrentTime;
            } else {
                // No data received within timeout
                Serial.println("{\"status\":\"no_sensor_data\"}");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

        
