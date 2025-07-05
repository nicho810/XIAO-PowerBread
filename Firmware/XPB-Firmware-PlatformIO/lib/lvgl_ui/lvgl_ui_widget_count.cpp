#include "lvgl_ui_widget_count.h"

// Data Count Widget Implementation
Widget_DataCount::Widget_DataCount(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark, lv_obj_t* parent) 
    : Widget_Base(x, y, color, color_dark), avg_seconds(0.0f), avg_minutes(0.0f), avg_AllTime(0.0f), peak_AllTime(0.0f) {
    
    // Create container
    container = lv_obj_create(parent);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    Widget_DataCount_item* item_avgS = new Widget_DataCount_item(0, -30, "avgS", unit_avgS, color, color_dark, container);
    Widget_DataCount_item* item_avgM = new Widget_DataCount_item(0, -10, "avgM", unit_avgM, color, color_dark, container);
    Widget_DataCount_item* item_avgAll = new Widget_DataCount_item(0, 10, "avgA", unit_avgAll, color, color_dark, container);
    Widget_DataCount_item* item_peakAll = new Widget_DataCount_item(0, 30, "Peak", unit_peakAll, color, color_dark, container);
}

void Widget_DataCount::addData_current(float data) {
    //todo: add data to the count
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
        snprintf(value_str, sizeof(value_str), "%.3f", value);
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

Widget_DataCount_item::~Widget_DataCount_item() {
    // Clean up any resources if needed
    // The base class destructor will be called automatically
}