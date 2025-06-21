#include "lvgl_ui_widget_chart.h"


// Data Chart Widget Implementation
Widget_DataChart::Widget_DataChart(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark) 
    : Widget_Base(x, y, color, color_dark), chart(nullptr), series(nullptr) {
    
    // Create container
    container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);

    // Create chart
    chart = lv_chart_create(container);
    lv_obj_set_size(chart, 78, 78);
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, 0);
    
    // Adjust padding
    lv_obj_set_style_pad_left(chart, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_right(chart, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_top(chart, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(chart, 4, LV_PART_MAIN);

    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_obj_set_style_border_width(chart, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(chart, color, LV_PART_MAIN);
    lv_obj_set_style_radius(chart, 4, LV_PART_MAIN);
    
    // Configure chart properties
    lv_chart_set_point_count(chart, 70);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_div_line_count(chart, 6, 0);
    lv_obj_set_style_line_color(chart, xpb_color_GridLines, LV_PART_MAIN);
    
    // Add dash pattern to grid lines
    static const lv_style_prop_t props[] = {LV_STYLE_PROP_INV};
    static lv_style_t style_grid;
    lv_style_init(&style_grid);
    lv_style_set_line_dash_width(&style_grid, 6);
    lv_style_set_line_dash_gap(&style_grid, 4);
    lv_obj_add_style(chart, &style_grid, LV_PART_MAIN);

    // Add data series
    series = lv_chart_add_series(chart, color, LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_series_color(chart, series, color);

    // Style modifications
    lv_obj_set_style_line_width(chart, 1, LV_PART_ITEMS);
    lv_obj_set_style_line_rounded(chart, true, LV_PART_ITEMS);
    lv_obj_set_style_bg_color(chart, xpb_color_Background, LV_PART_MAIN);
}

void Widget_DataChart::addDataPoint(int32_t value) {
    if (series) {
        lv_chart_set_next_value(chart, series, value);
    }
}

void Widget_DataChart::setRange(int32_t min, int32_t max) {
    if (chart) {
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
    }
}

void Widget_DataChart::setPointCount(uint16_t count) {
    if (chart) {
        lv_chart_set_point_count(chart, count);
    }
}

