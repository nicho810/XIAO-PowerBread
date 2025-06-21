#include "lvgl_ui_widget_monitor.h"



// Data Monitor Widget Implementation
Widget_DataMonitor::Widget_DataMonitor(uint16_t x, uint16_t y, const char* title, lv_color_t color) 
    : Widget_Base(x, y, color), title_label(nullptr), voltage_value(nullptr), voltage_unit(nullptr),
      current_value(nullptr), current_unit(nullptr), power_value(nullptr), power_unit(nullptr),
      title_rect(nullptr), title(title) {
    
    int pos_x_of_Voltage_lable = -10;
    int pos_y_of_Voltage_lable = -16;
    int pos_x_of_Voltage_unit = 10;
    int pos_y_of_Voltage_unit = pos_y_of_Voltage_lable + 3;

    int pos_x_of_Current_lable = pos_x_of_Voltage_lable;
    int pos_y_of_Current_lable = pos_y_of_Voltage_lable + 21;
    int pos_x_of_Current_unit = pos_x_of_Voltage_unit;
    int pos_y_of_Current_unit = pos_y_of_Current_lable + 3;

    int pos_x_of_Power_lable = pos_x_of_Voltage_lable;
    int pos_y_of_Power_lable = pos_y_of_Voltage_lable + 42;
    int pos_x_of_Power_unit = pos_x_of_Voltage_unit;
    int pos_y_of_Power_unit = pos_y_of_Power_lable + 3;

    // Create container
    container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    // Create title rectangle
    title_rect = lv_obj_create(container);
    lv_obj_clear_flag(title_rect, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(title_rect, 50, 12);
    lv_obj_align(title_rect, LV_ALIGN_TOP_LEFT, -14, -14);
    lv_obj_set_style_radius(title_rect, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title_rect, color, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_rect, 0, LV_PART_MAIN);

    // Create title
    title_label = lv_label_create(container);
    lv_obj_set_style_text_font(title_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, -10, -34);

    // Create voltage value
    voltage_value = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_value, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(voltage_value, "0.000");
    lv_obj_align(voltage_value, LV_ALIGN_LEFT_MID, pos_x_of_Voltage_lable, pos_y_of_Voltage_lable);

    // Create voltage unit
    voltage_unit = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(voltage_unit, "V");
    lv_obj_align(voltage_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Voltage_unit, pos_y_of_Voltage_unit);

    // Create current value
    current_value = lv_label_create(container);
    lv_obj_set_style_text_font(current_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_value, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(current_value, "0.000");
    lv_obj_align(current_value, LV_ALIGN_LEFT_MID, pos_x_of_Current_lable, pos_y_of_Current_lable);

    // Create current unit
    current_unit = lv_label_create(container);
    lv_obj_set_style_text_font(current_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(current_unit, "mA");
    lv_obj_align(current_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Current_unit, pos_y_of_Current_unit);

    // Create power value
    power_value = lv_label_create(container);
    lv_obj_set_style_text_font(power_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_value, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(power_value, "0.000");
    lv_obj_align(power_value, LV_ALIGN_LEFT_MID, pos_x_of_Power_lable, pos_y_of_Power_lable);

    // Create power unit
    power_unit = lv_label_create(container);
    lv_obj_set_style_text_font(power_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(power_unit, "mW");
    lv_obj_align(power_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Power_unit, pos_y_of_Power_unit);
}

void Widget_DataMonitor::setVoltage(float voltage) {
    if (voltage_value) {
        char voltage_str[16];
        snprintf(voltage_str, sizeof(voltage_str), "%.3f", voltage);
        lv_label_set_text(voltage_value, voltage_str);
    }
}

void Widget_DataMonitor::setCurrent(float current) {
    if (current_value) {
        char current_str[16];
        snprintf(current_str, sizeof(current_str), "%.3f", current);
        lv_label_set_text(current_value, current_str);
    }
}

void Widget_DataMonitor::setPower(float power) {
    if (power_value) {
        char power_str[16];
        snprintf(power_str, sizeof(power_str), "%.3f", power);
        lv_label_set_text(power_value, power_str);
    }
}

void Widget_DataMonitor::setVoltage(const char* voltage_str) {
    if (voltage_value) {
        lv_label_set_text(voltage_value, voltage_str);
    }
}

void Widget_DataMonitor::setCurrent(const char* current_str) {
    if (current_value) {
        lv_label_set_text(current_value, current_str);
    }
}

void Widget_DataMonitor::setPower(const char* power_str) {
    if (power_value) {
        lv_label_set_text(power_value, power_str);
    }
}

void Widget_DataMonitor::setTitle(const char* new_title) {
    title = new_title;
    if (title_label) {
        lv_label_set_text(title_label, title);
    }
}