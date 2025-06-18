#include "task_lvgl.h"

void lvglTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = 0;
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // 200Hz refresh rate


    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastWakeTime) >= xFrequency)
        {
            //Take the semaphore before accessing shared data
            if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                lv_task_handler();
                xLastWakeTime = xCurrentTime;
                xSemaphoreGive(lvglMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
