#pragma once
#include <lvgl.h>

// Forward declaration for LGFX
class LGFX;

// Extern LGFX instance (will be defined elsewhere)
extern LGFX tft;

// LVGL display flush callback
void xpb_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

// LVGL Input Device Keyboard read function
void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
