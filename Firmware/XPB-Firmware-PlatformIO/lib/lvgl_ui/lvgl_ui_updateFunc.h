/**
 * [INPUT]: lvgl.h, xpb_color_palette.h, INA3221Sensor.h (DualChannelData)
 * [OUTPUT]: 局部刷新函数 — update_monitor_data / update_chart_data / update_count_data / update_configMode
 * [POS]: UI 数据绑定层，将传感器数据 → LVGL widget 属性的映射，选择性刷新避免闪烁
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef LVGL_UI_UPDATEFUNC_H
#define LVGL_UI_UPDATEFUNC_H

#include "lvgl.h"
#include "xpb_color_palette.h"
#include "INA3221Sensor.h"

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData);
void update_chart_data(lv_obj_t *chart_container, int32_t new_value);
void update_count_data(lv_obj_t *count_container, uint8_t channel, float updateValue);  
void update_chart_range(lv_obj_t* chart, lv_chart_series_t* series);
void update_configMode(lv_obj_t *configMode_container, int8_t cursor, int8_t cursor_last, int8_t cursor_max, int8_t cursor_status);
void update_configMode_changeItemStatus(lv_obj_t *item, int8_t itemStatus);
void update_configMode_cfgData(lv_obj_t *item, int8_t cursor);

#endif
