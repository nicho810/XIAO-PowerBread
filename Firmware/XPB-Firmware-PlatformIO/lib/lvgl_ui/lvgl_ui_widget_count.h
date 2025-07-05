#ifndef LVGL_WIDGET_COUNT_H
#define LVGL_WIDGET_COUNT_H

#include "lvgl_ui_widget.h"


// Data Count Menu Widget item class
class Widget_DataCount_item : public Widget_Base {
    private:
        lv_obj_t* item_title_label;
        lv_obj_t* item_title_area;
        lv_obj_t* item_value_label;
        lv_obj_t* item_unit_label;
        lv_color_t item_color;
        lv_color_t item_color_dark;
        const char* item_title;
        const char* item_unit;
        float item_value;
    
    public:
        Widget_DataCount_item(uint16_t x, uint16_t y, const char* title, const char* unit, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent = nullptr);
        ~Widget_DataCount_item();
        void setValue(float value);
        void setValue(const char* value_str);
        void setUnit(const char* new_unit);
        void setTitle(const char* new_title);
};

// Data Count Widget Class
class Widget_DataCount : public Widget_Base {
    private:
        char unit_avgS[4] = "mA";
        char unit_avgM[4] = "mA";
        char unit_avgAll[4] = "mA";
        char unit_peakAll[4] = "mA";
        float avg_seconds; // average of last second
        float avg_minutes; // average of last minute
        float avg_AllTime; // average of all time
        float peak_AllTime; // peak of all time
    
    
    public:
        Widget_DataCount(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent = nullptr);
        void addData_current(float data);
        
    };

// // Data Count Menu Widget Class
// class Widget_DataCountMenu : public Widget_Base {
// private:
//     lv_obj_t* menu_item;
//     uint8_t current_highlight_channel;

// public:
//     Widget_DataCountMenu(uint16_t x, uint16_t y, uint8_t current_highlight_channel = 0);
    
//     void setHighlightChannel(uint8_t channel);
//     void show();
//     void hide();
//     bool isVisible() const;
// };

#endif