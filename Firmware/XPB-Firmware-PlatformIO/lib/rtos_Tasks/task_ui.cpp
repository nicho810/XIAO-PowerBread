#include "task_ui.h"

void uiTask(void* pvParameters)
{
    (void)pvParameters;
    TickType_t xLastUpdateTime = 0;
    const TickType_t xUpdateInterval = pdMS_TO_TICKS(100); // Update every 100ms

    // Initialize UI
    ui_manager.initUI(UI_Mode_DataMonitor);

    while (1) {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastUpdateTime) >= xUpdateInterval) {
            // Peek at the latest data from queue (non-blocking)
            SensorDataMessage receivedData;
            if (xQueuePeek(sensorDataQueue, &receivedData, pdMS_TO_TICKS(100)) == pdTRUE) {
                // Process the received data
                ui_manager.updateUI(UI_Mode_DataMonitor, receivedData, NULL);
            }
            xLastUpdateTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    
}