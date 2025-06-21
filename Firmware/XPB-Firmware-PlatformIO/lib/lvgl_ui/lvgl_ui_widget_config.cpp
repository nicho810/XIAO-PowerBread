#include "lvgl_ui_widget_config.h"



// Config Mode Item Widget Implementation
Widget_ConfigModeItem::Widget_ConfigModeItem(lv_obj_t* parent, uint16_t x, uint16_t y, const char* title_text, uint8_t value, uint8_t item_status) 
    : Widget_Base(x, y, xpb_color_Background), title_label(nullptr), value_box(nullptr), value_label(nullptr),
      title_text(title_text), value(value), item_status(item_status) {
    
    // Create container
    container = lv_obj_create(parent);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 70, 14);
    lv_obj_align(container, LV_ALIGN_TOP_LEFT, x, y);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

    // Create title label
    title_label = lv_label_create(container);
    lv_obj_set_style_text_font(title_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(title_label, title_text);
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, -10, 0);

    // Create value box
    value_box = lv_obj_create(container);
    lv_obj_clear_flag(value_box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(value_box, 22, 14);
    lv_obj_set_style_bg_color(value_box, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_radius(value_box, 4, LV_PART_MAIN);
    lv_obj_align(value_box, LV_ALIGN_RIGHT_MID, 10, 0);

    // Set initial border based on status
    if (item_status == 0) {
        lv_obj_set_style_border_width(value_box, 0, LV_PART_MAIN);
    } else if (item_status == 1) {
        lv_obj_set_style_border_width(value_box, 1, LV_PART_MAIN);
        lv_obj_set_style_border_color(value_box, xpb_color_Text, LV_PART_MAIN);
    } else if (item_status == 2) {
        lv_obj_set_style_border_width(value_box, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(value_box, xpb_color_ChannelA, LV_PART_MAIN);
    }

    // Create value label
    value_label = lv_label_create(value_box);
    lv_obj_set_style_text_font(value_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(value_label, xpb_color_Text, LV_PART_MAIN);
    char value_str[4];
    snprintf(value_str, sizeof(value_str), "%u", value);
    lv_label_set_text(value_label, value_str);
    lv_obj_align(value_label, LV_ALIGN_CENTER, 0, 0);
}

void Widget_ConfigModeItem::setValue(uint8_t new_value) {
    value = new_value;
    if (value_label) {
        char value_str[4];
        snprintf(value_str, sizeof(value_str), "%u", value);
        lv_label_set_text(value_label, value_str);
    }
}

void Widget_ConfigModeItem::setItemStatus(uint8_t status) {
    item_status = status;
    if (value_box) {
        if (status == 0) {
            lv_obj_set_style_border_width(value_box, 0, LV_PART_MAIN);
        } else if (status == 1) {
            lv_obj_set_style_border_width(value_box, 1, LV_PART_MAIN);
            lv_obj_set_style_border_color(value_box, xpb_color_Text, LV_PART_MAIN);
        } else if (status == 2) {
            lv_obj_set_style_border_width(value_box, 2, LV_PART_MAIN);
            lv_obj_set_style_border_color(value_box, xpb_color_ChannelA, LV_PART_MAIN);
        }
    }
}

void Widget_ConfigModeItem::setTitle(const char* new_title) {
    title_text = new_title;
    if (title_label) {
        lv_label_set_text(title_label, title_text);
    }
}