#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "xpb_color_palette.h"

lv_obj_t* dataMonitor_initUI(int rotation);
lv_obj_t* dataMonitorCount_initUI(int rotation, uint8_t channel);
lv_obj_t* dataMonitorChart_initUI(int rotation, uint8_t channel);

lv_obj_t* widget_DataMonitor_create(uint16_t x, uint16_t y, const char* title, lv_color_t color);
lv_obj_t* widget_DataCount_create(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark);
lv_obj_t* widget_DataChart_create(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark);
static void key_event_cb(lv_event_t * e);


#endif
