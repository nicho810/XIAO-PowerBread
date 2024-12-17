#include "lvgl_ui.h"
#include "xpb_color_palette.h"


void lvgl_ui_t1() {
    // Set up label with optimized settings
    lv_obj_t *counter_label;
    counter_label = lv_label_create(lv_scr_act());
    lv_label_set_text(counter_label, "0");
    lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(counter_label, lv_color_make(255, 0, 0), LV_PART_MAIN);
}

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
    // Create a container
    lv_obj_t* container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, 80, 80);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, -40);

    // Style the container
    lv_obj_set_style_radius(container, 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, lv_color_make(0, 0, 0), LV_PART_MAIN);        // Black background
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);                          // 2px border width
    lv_obj_set_style_border_color(container, xpb_color_ChannelA, LV_PART_MAIN);  // Red border color
    
    // Create a round rectangle inside the container
    lv_obj_t* roundRect = lv_obj_create(container);
    lv_obj_set_size(roundRect, 60, 60);  // Slightly smaller than container
    lv_obj_align(roundRect, LV_ALIGN_CENTER, 0, 0);
    
    // Style the round rectangle
    lv_obj_set_style_radius(roundRect, 10, LV_PART_MAIN);  // Round corners
    lv_obj_set_style_bg_color(roundRect, lv_color_make(100, 100, 100), LV_PART_MAIN);
    lv_obj_set_style_border_width(roundRect, 0, LV_PART_MAIN);  // No border
}

