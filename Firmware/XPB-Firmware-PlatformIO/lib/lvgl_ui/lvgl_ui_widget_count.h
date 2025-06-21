#ifndef LVGL_WIDGET_COUNT_H
#define LVGL_WIDGET_COUNT_H

#include "lvgl_ui_widget.h"


// Data Count Widget Class
class Widget_DataCount : public Widget_Base {
private:
    lv_obj_t* title_label;
    lv_obj_t* value_label;
    lv_obj_t* unit_label;
    lv_obj_t* title_background;
    const char* title;
    const char* unit;

public:
    Widget_DataCount(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark = xpb_color_Background, const char* unit = "mA");
    
    void setValue(float value);
    void setValue(const char* value_str);
    void setUnit(const char* new_unit);
    void setTitle(const char* new_title);
};

// Data Count Menu Widget Class
class Widget_DataCountMenu : public Widget_Base {
private:
    lv_obj_t* menu_item;
    uint8_t current_highlight_channel;

public:
    Widget_DataCountMenu(uint16_t x, uint16_t y, uint8_t current_highlight_channel = 0);
    
    void setHighlightChannel(uint8_t channel);
    void show();
    void hide();
    bool isVisible() const;
};

#endif