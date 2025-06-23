#include "task_ui.h"

void uiTask(void* pvParameters)
{
    (void)pvParameters;
    TickType_t xLastUpdateTime = 0;
    const TickType_t xUpdateInterval = pdMS_TO_TICKS(200); // Increase to 200ms

    // Initialize UI
    ui_manager.initUI(UI_Mode_DataMonitor);

    while (1) {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastUpdateTime) >= xUpdateInterval) {
            // Receive data from queue (blocking with timeout)
            SensorDataMessage receivedData;
            if (xQueueReceive(sensorDataQueue_ui, &receivedData, pdMS_TO_TICKS(50)) == pdTRUE) {
                // Process the received data with error handling
                try {
                    ui_manager.updateUI(UI_Mode_DataMonitor, receivedData, NULL);
                } catch (...) {
                    Serial.println("ERROR: Exception in UI task updateUI call!");
                }
            }
            lv_task_handler();  // Call lv_task_handler() to handle LVGL events
            xLastUpdateTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(20)); // Increase delay
    }
}