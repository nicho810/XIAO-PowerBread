#include "lvglTask.h"

void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // 200Hz refresh rate

    while (1)
    {
        if (xSemaphoreTake(lvglMutex, 0) == pdTRUE)
        { // Non-blocking mutex take
            lv_timer_handler();
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(xFrequency);
    }
}
