#ifndef LVGL_FUNC_H
#define LVGL_FUNC_H

#include <lvgl.h>
#include <LovyanGFX.h>
#if defined(Proj_XIAOPowerBread)
#include <LGFX_096_XPB.hpp>
#elif defined(Proj_XIAOPowerMonitor)
#endif
#include <LGFX_114_XPM.hpp>

// Extern LGFX instance (will be defined elsewhere)
extern LGFX tft;

// LVGL display flush callback
void lvgl_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void lvgl_init(void);

// LVGL Input Device Keyboard read function
// void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

#endif // LVGL_FUNC_H