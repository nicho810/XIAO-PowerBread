#include "lvgl_ui.h"
#include "xpb_color_palette.h"
// #include "inter_bold_18px_basicLatin.h"




void create_button_widget() {
    // Create a button
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
    
    // Add button style
    lv_obj_set_style_bg_color(btn, lv_color_make(100, 100, 100), LV_PART_MAIN);  // Add background color
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);  // Make background fully opaque
    
    // Create a label on the button
    lv_obj_t* btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Button");
    lv_obj_set_style_text_color(btn_label, lv_color_make(255, 255, 255), LV_PART_MAIN);  // White text
    lv_obj_center(btn_label);
}

void dataMonitor_initUI(int rotation) {
    lv_obj_t* dataMonitor_A = widget_DataMonitor_create(0, 41, "Channel A", xpb_color_ChannelA);
    lv_obj_t* dataMonitor_B = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);

}

void dataMonitorCount_initUI(int rotation, uint8_t channel) {
    lv_obj_t* dataMonitor_X;
    if (channel == 1) {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel A", xpb_color_ChannelA);
    } else if (channel == 2) {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);
    }

}

lv_obj_t* widget_DataCount_create(uint16_t x, uint16_t y, lv_color_t color) {
    lv_obj_t* container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, color, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);
    return container;
}

lv_obj_t* widget_DataMonitor_create(uint16_t x, uint16_t y, const char* title_text, lv_color_t color) {

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

    // Create a container
    lv_obj_t* container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);  // 禁用容器的滚动功能
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);        // Black background
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);                          // 2px border width
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);  // Red border color

    // Create a title rectangle in top-left corner
    lv_obj_t* title_rect = lv_obj_create(container);
    lv_obj_clear_flag(title_rect, LV_OBJ_FLAG_SCROLLABLE);  // 禁用标题矩形的滚动功能
    lv_obj_set_size(title_rect, 50, 12);
    lv_obj_align(title_rect, LV_ALIGN_TOP_LEFT, -14, -14);
    lv_obj_set_style_radius(title_rect, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title_rect, color, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_rect, 0, LV_PART_MAIN);

    // Create a title on the container
    lv_obj_t* title = lv_label_create(container);
    lv_obj_set_style_text_font(title, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, xpb_color_Text, LV_PART_MAIN);  // White tex
    lv_label_set_text(title, title_text);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, -10, -34);

    // Create a Voltage label on the container
    lv_obj_t* voltage_value = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_value, xpb_color_Text, LV_PART_MAIN);  // White tex
    lv_label_set_text(voltage_value, "0.000");
    lv_obj_align(voltage_value, LV_ALIGN_LEFT_MID, pos_x_of_Voltage_lable, pos_y_of_Voltage_lable);

    // Create a Voltage unit label on the container
    lv_obj_t* voltage_unit = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(voltage_unit, "V");
    lv_obj_align(voltage_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Voltage_unit, pos_y_of_Voltage_unit);

    // Create a Current label on the container
    lv_obj_t* current_value = lv_label_create(container);
    lv_obj_set_style_text_font(current_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_value, xpb_color_Text, LV_PART_MAIN);  // White tex
    lv_label_set_text(current_value, "0.000");
    lv_obj_align(current_value, LV_ALIGN_LEFT_MID, pos_x_of_Current_lable, pos_y_of_Current_lable);

    // Create a Current unit label on the container
    lv_obj_t* current_unit = lv_label_create(container);
    lv_obj_set_style_text_font(current_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(current_unit, "mA");
    lv_obj_align(current_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Current_unit, pos_y_of_Current_unit);

    // Create a Power label on the container
    lv_obj_t* power_value = lv_label_create(container);
    lv_obj_set_style_text_font(power_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_value, xpb_color_Text, LV_PART_MAIN);  // White tex
    lv_label_set_text(power_value, "0.000");
    lv_obj_align(power_value, LV_ALIGN_LEFT_MID, pos_x_of_Power_lable, pos_y_of_Power_lable);

    // Create a Power unit label on the container
    lv_obj_t* power_unit = lv_label_create(container);
    lv_obj_set_style_text_font(power_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(power_unit, "mW");
    lv_obj_align(power_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Power_unit, pos_y_of_Power_unit);

    return container;
}