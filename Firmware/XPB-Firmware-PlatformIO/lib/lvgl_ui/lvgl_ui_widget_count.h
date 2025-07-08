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
        
        // Helper function to format truncated value for display
        void formatTruncatedValue(float value, char* out_str, size_t out_str_size);
    
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
        
        // Time-based rolling average data structures
        static const uint16_t MAX_SAMPLES_PER_SECOND = 10; // 10 samples per second (100ms interval)
        static const uint16_t MAX_SAMPLES_PER_MINUTE = 600; // 600 samples per minute
        
        // Circular buffers for rolling averages
        float second_buffer[MAX_SAMPLES_PER_SECOND];
        float minute_buffer[MAX_SAMPLES_PER_MINUTE];
        uint32_t second_timestamps[MAX_SAMPLES_PER_SECOND];
        uint32_t minute_timestamps[MAX_SAMPLES_PER_MINUTE];
        uint16_t second_buffer_index;
        uint16_t minute_buffer_index;
        uint16_t second_buffer_count;
        uint16_t minute_buffer_count;
        
        // Running totals for efficient calculation
        float second_sum;
        float minute_sum;
        
        // All-time statistics
        float avg_AllTime; // average of all time
        float peak_AllTime; // peak of all time
        uint32_t avg_AllTime_count; // count of all time
        
        Widget_DataCount_item* item_avgS;
        Widget_DataCount_item* item_avgM;
        Widget_DataCount_item* item_avgAll;
        Widget_DataCount_item* item_peakAll;
    
    public:
        Widget_DataCount(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent = nullptr);
        void addData_current(float data);
        
    };


#endif