#include "lvgl_ui_widget.h"

// Base Widget Implementation
Widget_Base::Widget_Base(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark) 
    : x(x), y(y), color(color), color_dark(color_dark), container(nullptr) {
}

void Widget_Base::setPosition(uint16_t new_x, uint16_t new_y) {
    x = new_x;
    y = new_y;
    if (container) {
        lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    }
}

void Widget_Base::setColor(lv_color_t new_color) {
    color = new_color;
    if (container) {
        lv_obj_set_style_border_color(container, color, LV_PART_MAIN);
    }
}

void Widget_Base::setVisibility(bool visible) {
    if (container) {
        if (visible) {
            lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}




// // Legacy function wrappers for backward compatibility
// lv_obj_t *widget_DataChart_create(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark) {
//     Widget_DataChart* widget = new Widget_DataChart(x, y, color, color_dark);
//     return widget->getContainer();
// }

// lv_obj_t* widget_DataMonitor_create(uint16_t x, uint16_t y, const char* title, lv_color_t color) {
//     Widget_DataMonitor* widget = new Widget_DataMonitor(x, y, title, color);
//     return widget->getContainer();
// }

// lv_obj_t* widget_DataCount_create(uint16_t x, uint16_t y, const char* title, lv_color_t color, lv_color_t color_dark) {
//     Widget_DataCount* widget = new Widget_DataCount(x, y, title, color, color_dark);
//     return widget->getContainer();
// }

// lv_obj_t* widget_DataCount_Menu_create(uint16_t x, uint16_t y, uint8_t current_highLight_channel) {
//     Widget_DataCountMenu* widget = new Widget_DataCountMenu(x, y, current_highLight_channel);
//     return widget->getContainer();
// }

// lv_obj_t* widget_configMode_item(lv_obj_t *parent, uint16_t x, uint16_t y, const char *title_text, uint8_t value, uint8_t itemStatus) {
//     Widget_ConfigModeItem* widget = new Widget_ConfigModeItem(parent, x, y, title_text, value, itemStatus);
//     return widget->getContainer();
// }