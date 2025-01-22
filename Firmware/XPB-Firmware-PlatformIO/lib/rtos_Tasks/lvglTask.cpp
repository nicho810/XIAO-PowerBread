#include "lvglTask.h"

void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // 200Hz refresh rate
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
        { 
            // Process any pending LVGL tasks
            lv_timer_handler();
            
            // Force LVGL to process input devices
            lv_indev_t * indev = lv_indev_get_next(NULL);
            while(indev) {
                lv_indev_data_t data;
                _lv_indev_read(indev, &data);  // Fixed: added data parameter
                indev = lv_indev_get_next(indev);
            }
            
            xSemaphoreGive(lvglMutex);
        }
        
        // Use vTaskDelayUntil for more precise timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
