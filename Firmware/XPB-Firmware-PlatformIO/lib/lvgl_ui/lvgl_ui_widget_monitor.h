#ifndef LVGL_WIDGET_MONITOR_H
#define LVGL_WIDGET_MONITOR_H

#include "lvgl_ui_widget.h"

// Data Monitor Widget Class
class Widget_DataMonitor : public Widget_Base {
private:
    lv_obj_t* title_label;
    lv_obj_t* voltage_value;
    lv_obj_t* voltage_unit;
    lv_obj_t* current_value;
    lv_obj_t* current_unit;
    lv_obj_t* power_value;
    lv_obj_t* power_unit;
    lv_obj_t* title_rect;
    const char* title;

public:
    Widget_DataMonitor(uint16_t x, uint16_t y, const char* title, lv_color_t color);
    
    void setVoltage(float voltage);
    void setCurrent(float current);
    void setPower(float power);
    void setVoltage(const char* voltage_str);
    void setCurrent(const char* current_str);
    void setPower(const char* power_str);
    void setTitle(const char* new_title);
};


#endif