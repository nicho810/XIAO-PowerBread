#include "lvgl_ui_widget_count.h"

// Data Count Widget Implementation
Widget_DataCount::Widget_DataCount(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent) 
    : Widget_Base(x, y, color, color_dark), 
      second_buffer_index(0), minute_buffer_index(0), 
      second_buffer_count(0), minute_buffer_count(0),
      second_sum(0.0f), minute_sum(0.0f),
      avg_AllTime(0.0f), peak_AllTime(0.0f), avg_AllTime_count(0) {
    
    // Initialize circular buffers
    for (int i = 0; i < MAX_SAMPLES_PER_SECOND; i++) {
        second_buffer[i] = 0.0f;
        second_timestamps[i] = 0;
    }
    for (int i = 0; i < MAX_SAMPLES_PER_MINUTE; i++) {
        minute_buffer[i] = 0.0f;
        minute_timestamps[i] = 0;
    }
    
    // Create container
    container = lv_obj_create(parent);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    item_avgS = new Widget_DataCount_item(0, -30, "avgS", unit_avgS, color, color_dark, container);
    item_avgM = new Widget_DataCount_item(0, -10, "avgM", unit_avgM, color, color_dark, container);
    item_avgAll = new Widget_DataCount_item(0, 10, "avgA", unit_avgAll, color, color_dark, container);
    item_peakAll = new Widget_DataCount_item(0, 30, "Peak", unit_peakAll, color, color_dark, container);

    //Set the ui to 0 for initial value
    item_avgS->setValue(0.0f);
    item_avgM->setValue(0.0f);
    item_avgAll->setValue(0.0f);
    item_peakAll->setValue(0.0f);
}

void Widget_DataCount::addData_current(float data) {
    // Truncate input data to 4 digits
    data = trunc(data * 10000) / 10000;
    
    // Get current timestamp in milliseconds
    uint32_t current_time = millis();
    
    // Update second buffer (rolling 1-second window)
    // First, remove old data that's older than 1 second
    uint16_t valid_second_count = 0;
    float new_second_sum = 0.0f;
    
    // Scan through all data points and keep only those within the last 1 second
    for (uint16_t i = 0; i < second_buffer_count; i++) {
        uint16_t idx = (second_buffer_index - second_buffer_count + i + MAX_SAMPLES_PER_SECOND) % MAX_SAMPLES_PER_SECOND;
        if (current_time - second_timestamps[idx] < 1000) {
            // This data point is still valid (less than 1 second old)
            new_second_sum += second_buffer[idx];
            valid_second_count++;
        }
    }
    
    // Add new data point
    second_buffer[second_buffer_index] = data;
    second_timestamps[second_buffer_index] = current_time;
    new_second_sum += data;
    valid_second_count++;
    
    // Update buffer state
    second_sum = new_second_sum;
    second_buffer_count = valid_second_count;
    second_buffer_index = (second_buffer_index + 1) % MAX_SAMPLES_PER_SECOND;
    
    // Update minute buffer (rolling 1-minute window)
    // First, remove old data that's older than 60 seconds
    uint16_t valid_minute_count = 0;
    float new_minute_sum = 0.0f;
    
    // Scan through all data points and keep only those within the last 60 seconds
    for (uint16_t i = 0; i < minute_buffer_count; i++) {
        uint16_t idx = (minute_buffer_index - minute_buffer_count + i + MAX_SAMPLES_PER_MINUTE) % MAX_SAMPLES_PER_MINUTE;
        if (current_time - minute_timestamps[idx] < 60000) {
            // This data point is still valid (less than 60 seconds old)
            new_minute_sum += minute_buffer[idx];
            valid_minute_count++;
        }
    }
    
    // Add new data point
    minute_buffer[minute_buffer_index] = data;
    minute_timestamps[minute_buffer_index] = current_time;
    new_minute_sum += data;
    valid_minute_count++;
    
    // Update buffer state
    minute_sum = new_minute_sum;
    minute_buffer_count = valid_minute_count;
    minute_buffer_index = (minute_buffer_index + 1) % MAX_SAMPLES_PER_MINUTE;
    
    // Calculate rolling averages
    float avg_seconds = (second_buffer_count > 0) ? second_sum / second_buffer_count : 0.0f;
    float avg_minutes = (minute_buffer_count > 0) ? minute_sum / minute_buffer_count : 0.0f;
    
    // Truncate averages to 4 digits
    avg_seconds = trunc(avg_seconds * 10000) / 10000;
    avg_minutes = trunc(avg_minutes * 10000) / 10000;
    
    // Calculate average of all time (continuous accumulation)
    avg_AllTime_count++;
    avg_AllTime = ((avg_AllTime * (avg_AllTime_count - 1)) + data) / avg_AllTime_count;
    avg_AllTime = trunc(avg_AllTime * 10000) / 10000;
    
    // Calculate peak of all time
    if (data > peak_AllTime) {
        peak_AllTime = data;
        peak_AllTime = trunc(peak_AllTime * 10000) / 10000;
    }

    // Update the UI
    item_avgS->setValue(avg_seconds);
    item_avgM->setValue(avg_minutes);
    item_avgAll->setValue(avg_AllTime);
    item_peakAll->setValue(peak_AllTime);
}

// Data Count Menu Widget item Implementation
Widget_DataCount_item::Widget_DataCount_item(uint16_t x, uint16_t y, const char* title, const char* unit, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent) 
    : Widget_Base(x, y, color, color_dark), item_title_label(nullptr), item_value_label(nullptr), item_unit_label(nullptr), item_color(color), item_color_dark(color_dark),
      item_title(title), item_unit(unit), item_value(0.0f) {

    // Create container
    container = lv_obj_create(parent);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 18);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    // Create title area
    item_title_area = lv_obj_create(container);
    lv_obj_clear_flag(item_title_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(item_title_area, 30, 18);
    lv_obj_align(item_title_area, LV_ALIGN_LEFT_MID, -13, 0);
    lv_obj_set_style_radius(item_title_area, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(item_title_area, color_dark, LV_PART_MAIN);
    lv_obj_set_style_border_width(item_title_area, 0, LV_PART_MAIN);

    // Create title label
    item_title_label = lv_label_create(container);
    lv_obj_set_style_text_font(item_title_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(item_title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(item_title_label, title);
    lv_obj_align(item_title_label, LV_ALIGN_LEFT_MID, -10, -1);

    // Create value label
    item_value_label = lv_label_create(container);
    lv_obj_set_style_text_font(item_value_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(item_value_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(item_value_label, "0.000");
    lv_obj_align(item_value_label, LV_ALIGN_LEFT_MID, 20, 0);

    // Create unit label
    item_unit_label = lv_label_create(container);
    lv_obj_set_style_text_font(item_unit_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(item_unit_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(item_unit_label, unit);
    lv_obj_align(item_unit_label, LV_ALIGN_RIGHT_MID, 10, 0);
}

void Widget_DataCount_item::setValue(float value) {
    if (item_value_label) {
        char value_str[16];
        formatTruncatedValue(value, value_str, sizeof(value_str));
        lv_label_set_text(item_value_label, value_str);
    }
}

void Widget_DataCount_item::setValue(const char* value_str) {
    if (item_value_label) {
        lv_label_set_text(item_value_label, value_str);
    }
}

void Widget_DataCount_item::setUnit(const char* new_unit) {
    item_unit = new_unit;
    if (item_unit_label) {
        lv_label_set_text(item_unit_label, item_unit);
    }
}

void Widget_DataCount_item::setTitle(const char* new_title) {
    item_title = new_title;
    if (item_title_label) {
        lv_label_set_text(item_title_label, item_title);
    }
}

// Helper function to format truncated value for display
void Widget_DataCount_item::formatTruncatedValue(float value, char* out_str, size_t out_str_size) {
    if (value >= 1000.0f) {
        int truncated = (int)value;
        snprintf(out_str, out_str_size, "%d", truncated);
    } else if (value >= 100.0f) {
        float truncated = (float)((int)(value * 10)) / 10.0f;
        snprintf(out_str, out_str_size, "%.1f", truncated);
    } else if (value >= 10.0f) {
        float truncated = (float)((int)(value * 100)) / 100.0f;
        snprintf(out_str, out_str_size, "%.2f", truncated);
    } else {
        float truncated = (float)((int)(value * 1000)) / 1000.0f;
        snprintf(out_str, out_str_size, "%.3f", truncated);
    }
}

Widget_DataCount_item::~Widget_DataCount_item() {
    // Clean up any resources if needed
    // The base class destructor will be called automatically
}