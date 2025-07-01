#include "task_ui.h"

void uiTask(void* pvParameters)
{
    (void)pvParameters;
    TickType_t xLastUpdateTime = 0;
    const TickType_t xUpdateInterval = pdMS_TO_TICKS(10); // Increase to 200ms

    // Initialize UI
    // ui_manager.initUI(UI_Mode_DataMonitor);
    ui_manager.initUI(UI_Mode_DataChart_1);


    while (1) {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastUpdateTime) >= xUpdateInterval) {
            // Handle sensor data and update UI
            SensorDataMessage receivedData;

            //check of the mode is changed
            if (ui_manager.getCurrentMode() != ui_manager.getPreviousMode()) {
                ui_manager.initUI(ui_manager.getCurrentMode());
            }

            //update UI data based on the mode
            if (xQueueReceive(sensorDataQueue_ui, &receivedData, pdMS_TO_TICKS(50)) == pdTRUE) {
                // Process the received data with error handling
                try {
                    switch (ui_manager.getCurrentMode()) {
                        case UI_Mode_DataMonitor:
                            ui_manager.updateUI(UI_Mode_DataMonitor, receivedData, NULL);
                            break;
                        case UI_Mode_DataChart_1:
                            ui_manager.updateUI(UI_Mode_DataChart_1, receivedData, NULL);
                            break;
                    }
                } catch (...) {
                    Serial.println("ERROR: Exception in UI task updateUI call!");
                }
            }

            // Handle LVGL events
            lv_task_handler();  // Call lv_task_handler() to handle LVGL events
            xLastUpdateTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(20)); // Increase delay
    }
}