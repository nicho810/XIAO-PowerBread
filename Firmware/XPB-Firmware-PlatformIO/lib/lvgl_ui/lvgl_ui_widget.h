#ifndef LVGL_WIDGET_H
#define LVGL_WIDGET_H

#include "Arduino.h"
#include "lvgl.h"
#include "colorPalette.h"
#include "boardConfig.h"
#include <cstdio>

// Base widget class
class Widget_Base {
protected:
    lv_obj_t* container;
    lv_color_t color;
    lv_color_t color_dark;
    uint16_t x, y;

public:
    Widget_Base(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark = xpb_color_Background);
    virtual ~Widget_Base() = default;
    
    virtual lv_obj_t* getContainer() const { return container; }
    virtual void setPosition(uint16_t new_x, uint16_t new_y);
    virtual void setColor(lv_color_t new_color);
    virtual void setVisibility(bool visible);
};


// Legacy function wrappers for backward compatibility
// lv_obj_t *widget_DataChart_create(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark);
// lv_obj_t* widget_DataMonitor_create(uint16_t x, uint16_t y, const char* title, lv_color_t color);
// lv_obj_t* widget_DataCount_create(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark);
// lv_obj_t* widget_DataCount_Menu_create(uint16_t x, uint16_t y, uint8_t current_highLight_channel);
// lv_obj_t* widget_configMode_item(lv_obj_t *parent, uint16_t x, uint16_t y, const char *title_text, uint8_t value, uint8_t itemStatus);



#endif
