#ifndef LVGL_UI_UPDATEFUNC_H
#define LVGL_UI_UPDATEFUNC_H

#include "lvgl.h"
#include "xpb_color_palette.h"
#include "currentSensor.h"

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);
void update_chart_data(lv_obj_t *chart_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);
void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);  
void update_chart_range(lv_obj_t* chart, lv_chart_series_t* series);
void update_configMode(lv_obj_t *configMode_container, int8_t cursor, int8_t cursor_last, int8_t cursor_max, int8_t cursor_status);
void update_configMode_changeItemStatus(lv_obj_t *item, int8_t itemStatus);
void update_configMode_cfgData(lv_obj_t *item, int8_t cursor);

#endif
