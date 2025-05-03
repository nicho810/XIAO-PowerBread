#include "lvgl_func.h"
#include "LGFX_114_XPM.hpp"

// Extern LGFX instance (defined in main file)
extern LGFX tft;

// Add these externs at the top if needed:
extern SemaphoreHandle_t xSemaphore;
extern volatile bool last_key_pressed;
extern volatile uint32_t last_key;

void lvgl_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t len = w * h;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    // Replace individual pushColor with bulk writePixels
    tft.writePixels((uint16_t *)&color_p->full, len, true);

    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    // Take semaphore with a short timeout
    if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        if (last_key_pressed)
        {
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = last_key;
        }
        else
        {
            data->state = LV_INDEV_STATE_RELEASED;
            data->key = last_key;
        }

        // Reset the key press state after it's been read
        last_key_pressed = false;

        xSemaphoreGive(xSemaphore);
    }
    else
    {
        // If we couldn't get the semaphore, maintain previous state
        data->state = LV_INDEV_STATE_RELEASED;
        data->key = 0;
    }
}