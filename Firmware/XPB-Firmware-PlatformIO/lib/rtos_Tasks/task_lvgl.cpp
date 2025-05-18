#include "task_lvgl.h"

void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // 200Hz refresh rate
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
        { 
            // Process any pending LVGL tasks
            lv_timer_handler();  // This internally handles input device reading
            
            // Force a display refresh if needed
            lv_disp_t * disp = lv_disp_get_default();
            if(disp) {
                lv_refr_now(disp);
            }
            
            xSemaphoreGive(lvglMutex);
        }
        
        // Use vTaskDelayUntil for more precise timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
