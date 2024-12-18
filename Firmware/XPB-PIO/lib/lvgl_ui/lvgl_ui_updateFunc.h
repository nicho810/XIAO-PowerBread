#ifndef LVGL_UI_UPDATEFUNC_H
#define LVGL_UI_UPDATEFUNC_H

#include "lvgl.h"
#include "INA3221Sensor.h"

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData);
void update_chart_data(lv_obj_t *chart_container, int32_t new_value);
void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData);   

#endif