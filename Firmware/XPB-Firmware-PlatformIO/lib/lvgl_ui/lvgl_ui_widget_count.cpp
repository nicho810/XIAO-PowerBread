#include "lvgl_ui_widget_count.h"

// Data Count Widget Implementation
Widget_DataCount::Widget_DataCount(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark, const char* unit) 
    : Widget_Base(x, y, color, color_dark), title_label(nullptr), value_label(nullptr), unit_label(nullptr), 
      title_background(nullptr), title(title), unit(unit) {
    
    // Create container
    container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 18);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    // Create title background
    title_background = lv_obj_create(container);
    lv_obj_clear_flag(title_background, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(title_background, 30, 18);
    lv_obj_align(title_background, LV_ALIGN_LEFT_MID, -13, 0);
    lv_obj_set_style_radius(title_background, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title_background, color_dark, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_background, 0, LV_PART_MAIN);

    // Create title label
    title_label = lv_label_create(container);
    lv_obj_set_style_text_font(title_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, -10, -1);

    // Create value label
    value_label = lv_label_create(container);
    lv_obj_set_style_text_font(value_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(value_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(value_label, "0.000");
    lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 20, 0);

    // Create unit label
    unit_label = lv_label_create(container);
    lv_obj_set_style_text_font(unit_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(unit_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(unit_label, unit);
    lv_obj_align(unit_label, LV_ALIGN_RIGHT_MID, 10, 0);
}

void Widget_DataCount::setValue(float value) {
    if (value_label) {
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.3f", value);
        lv_label_set_text(value_label, value_str);
    }
}

void Widget_DataCount::setValue(const char* value_str) {
    if (value_label) {
        lv_label_set_text(value_label, value_str);
    }
}

void Widget_DataCount::setUnit(const char* new_unit) {
    unit = new_unit;
    if (unit_label) {
        lv_label_set_text(unit_label, unit);
    }
}

void Widget_DataCount::setTitle(const char* new_title) {
    title = new_title;
    if (title_label) {
        lv_label_set_text(title_label, title);
    }
}



// Data Count Menu Widget Implementation
Widget_DataCountMenu::Widget_DataCountMenu(uint16_t x, uint16_t y, uint8_t current_highlight_channel) 
    : Widget_Base(x, y, xpb_color_Background), menu_item(nullptr), current_highlight_channel(current_highlight_channel) {
    
    lv_color_t this_channel_color;
    if (current_highlight_channel == 0) {
        this_channel_color = xpb_color_ChannelA;
    } else if (current_highlight_channel == 1) {
        this_channel_color = xpb_color_ChannelB;
    }

    // Create container
    container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 80, 160);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(container, LV_OPA_70, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

    // Create menu item
    menu_item = lv_obj_create(container);
    lv_obj_set_size(menu_item, 40, 40);
    lv_obj_align(menu_item, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_style_bg_color(menu_item, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(menu_item, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_border_width(menu_item, 4, LV_PART_MAIN);
    lv_obj_set_style_border_color(menu_item, this_channel_color, LV_PART_MAIN);

    // Make hidden by default
    lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
}

void Widget_DataCountMenu::setHighlightChannel(uint8_t channel) {
    current_highlight_channel = channel;
    lv_color_t this_channel_color;
    if (channel == 0) {
        this_channel_color = xpb_color_ChannelA;
    } else if (channel == 1) {
        this_channel_color = xpb_color_ChannelB;
    }
    
    if (menu_item) {
        lv_obj_set_style_border_color(menu_item, this_channel_color, LV_PART_MAIN);
    }
}

void Widget_DataCountMenu::show() {
    if (container) {
        lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
    }
}

void Widget_DataCountMenu::hide() {
    if (container) {
        lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
    }
}

bool Widget_DataCountMenu::isVisible() const {
    if (container) {
        return !(lv_obj_has_flag(container, LV_OBJ_FLAG_HIDDEN));
    }
    return false;
}