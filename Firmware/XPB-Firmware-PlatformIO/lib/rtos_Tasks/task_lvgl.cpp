#include "task_lvgl.h"

extern volatile bool ui_initialization_in_progress;

void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // 200Hz refresh rate
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        if (!ui_initialization_in_progress) {  // Only process UI updates if not initializing
            if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                lv_task_handler();
                xSemaphoreGive(lvglMutex);
            }
        }
        
        // Use vTaskDelayUntil for more precise timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
