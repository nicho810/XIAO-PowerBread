#ifndef LVGL_FUNC_H
#define LVGL_FUNC_H

#include "lvgl_ui.h"

#include <lvgl.h>
#include <LovyanGFX.h>
#if defined(Proj_XIAOPowerBread)
#include <LGFX_096_XPB.hpp>
#elif defined(Proj_XIAOPowerMonitor)
#include <LGFX_114_XPM.hpp>
#endif

#include "input_ButtonX3.h"

//  // it cause circular dependency, dont know how to fix it yet
// class UI_manager; // forward declaration
// enum UI_Mode; // forward declaration



// Extern LGFX instance (will be defined elsewhere)
extern LGFX tft;

// LVGL display flush callback
void lvgl_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void lvgl_init(void);

// LVGL Input Device Keyboard read function
void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
void setup_container_events(lv_obj_t *container);
void key_event_cb(lv_event_t *e);

#endif // LVGL_FUNC_H