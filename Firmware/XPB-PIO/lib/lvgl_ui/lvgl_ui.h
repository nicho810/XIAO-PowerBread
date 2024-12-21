#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "xpb_color_palette.h"
#include "lvgl_widget.h"

lv_obj_t* dataMonitor_initUI(int rotation);
lv_obj_t* dataMonitorCount_initUI(int rotation, uint8_t channel);
lv_obj_t* dataMonitorChart_initUI(int rotation, uint8_t channel);


static void key_event_cb(lv_event_t * e);


#endif
