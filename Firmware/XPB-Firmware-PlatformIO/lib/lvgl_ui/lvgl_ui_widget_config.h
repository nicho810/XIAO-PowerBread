#ifndef LVGL_WIDGET_CONFIG_H
#define LVGL_WIDGET_CONFIG_H

#include "lvgl_ui_widget.h"

// Config Mode Item Widget Class
class Widget_ConfigModeItem : public Widget_Base {
private:
    lv_obj_t* title_label;
    lv_obj_t* value_box;
    lv_obj_t* value_label;
    const char* title_text;
    uint8_t value;
    uint8_t item_status;

public:
    Widget_ConfigModeItem(lv_obj_t* parent, uint16_t x, uint16_t y, const char* title_text, uint8_t value, uint8_t item_status = 0);
    
    void setValue(uint8_t new_value);
    void setItemStatus(uint8_t status);
    void setTitle(const char* new_title);
    uint8_t getValue() const { return value; }
    uint8_t getItemStatus() const { return item_status; }
};

#endif
