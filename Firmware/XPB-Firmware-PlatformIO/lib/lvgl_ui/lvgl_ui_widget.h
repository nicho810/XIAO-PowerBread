#ifndef LVGL_WIDGET_H
#define LVGL_WIDGET_H

#include "lvgl.h"
#include "xpb_color_palette.h"
#include <cstdio>

lv_obj_t *widget_DataChart_create(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark);
lv_obj_t* widget_DataMonitor_create(uint16_t x, uint16_t y, const char* title, lv_color_t color);
lv_obj_t* widget_DataCount_create(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark);
lv_obj_t* widget_DataCount_Menu_create(uint16_t x, uint16_t y, uint8_t current_highLight_channel);
lv_obj_t* widget_configMode_item(lv_obj_t *parent, uint16_t x, uint16_t y, const char *title_text, uint8_t value, uint8_t itemStatus);

#endif
