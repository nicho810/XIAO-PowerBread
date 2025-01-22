#include "lvgl_ui_updateFunc.h"
#include "INA3221Sensor.h"

static void format_value(char* buf, size_t buf_size, float value) {
    int32_t val100 = (int32_t)(value * 100.0f);
    if (val100 >= 10000) {
        snprintf(buf, buf_size, "%0.1f", value);
    } else if (val100 >= 1000) {
        snprintf(buf, buf_size, "%0.2f", value);
    } else {
        snprintf(buf, buf_size, "%0.3f", value);
    }
}

void update_chart_data(lv_obj_t *chart_container, int32_t new_value)
{
    if (!chart_container) return;

    // Get chart directly - it's the first child
    lv_obj_t *chart = lv_obj_get_child(chart_container, 0);
    if (!chart) return;

    // Get series directly - we know there's only one
    lv_chart_series_t *series = lv_chart_get_series_next(chart, NULL);
    if (!series) return;
    
    lv_chart_set_next_value(chart, series, new_value);
    update_chart_range(chart, series);
    lv_chart_refresh(chart);
}

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData)
{
    if (!monitor_container) return;

    static char str_buf[8];  // Single buffer for all string conversions
    
    const INAData* data = channel ? &newSensorData.channel1 : &newSensorData.channel0;

    // Direct child access using indices
    lv_obj_t* voltage_label = lv_obj_get_child(monitor_container, 2);
    lv_obj_t* current_label = lv_obj_get_child(monitor_container, 4);
    lv_obj_t* power_label = lv_obj_get_child(monitor_container, 6);

    if (!voltage_label || !current_label || !power_label) return;
    
    format_value(str_buf, sizeof(str_buf), data->busVoltage);
    lv_label_set_text(voltage_label, str_buf);

    format_value(str_buf, sizeof(str_buf), data->busCurrent);
    lv_label_set_text(current_label, str_buf);

    format_value(str_buf, sizeof(str_buf), data->busPower);
    lv_label_set_text(power_label, str_buf);
}

void update_count_data(lv_obj_t *count_container, uint8_t channel, float updateValue)
{
    if (!count_container) return;

    static char str_buf[8];  // Single buffer for all string conversions

    // Get value label - it's the second child (index 1)
    lv_obj_t* value_label = lv_obj_get_child(count_container, 2);
    if (!value_label) return;
    
    format_value(str_buf, sizeof(str_buf), updateValue);
    lv_label_set_text(value_label, str_buf);
}

void update_chart_range(lv_obj_t* chart, lv_chart_series_t* series) {
    // Find max value (min will be fixed at 0)
    int32_t max = INT32_MIN;
    uint16_t point_count = lv_chart_get_point_count(chart);
    
    for(uint16_t i = 0; i < point_count; i++) {
        int32_t value = series->y_points[i];
        if(value != LV_CHART_POINT_NONE) {
            if(value > max) max = value;
        }
    }
    
    // Add 10% padding to the top only
    max += max / 10;
    
    // Update chart range with fixed minimum at 0
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, max);
}


void update_configMode(lv_obj_t *item_area, int8_t cursor, int8_t cursor_status)
{
    if (!item_area) return;

    for (int i = 0; i < 15; i++) {
        lv_obj_t *item = lv_obj_get_child(item_area, i);
        //get the child of the item, which is the value_box
        lv_obj_t *value_box = lv_obj_get_child(item, 1);
        if (i == cursor && cursor_status == 0) {  // when the cursor = item_id, and cursor_status = 0(unselected), change the itemStatus to 1(Hover)
            update_configMode_changeItemStatus(value_box, 1);
        } else if (i == cursor && cursor_status == 1) {  // when the cursor = item_id, and cursor_status = 1(selected), change the itemStatus to 2(Selected)
            update_configMode_changeItemStatus(value_box, 2);
        } else { // when the cursor != item_id, change the itemStatus to 0(unselected)
            update_configMode_changeItemStatus(value_box, 0);
        }
    }
    
}

void update_configMode_changeItemStatus(lv_obj_t *item, int8_t itemStatus){
    if (!item) return;

    if (itemStatus == 0) {
        lv_obj_set_style_border_width(item, 0, LV_PART_MAIN);
    } else if (itemStatus == 1) {
        lv_obj_set_style_border_width(item, 1, LV_PART_MAIN);
        lv_obj_set_style_border_color(item, xpb_color_Text, LV_PART_MAIN);
    } else if (itemStatus == 2) {
        lv_obj_set_style_border_width(item, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item, xpb_color_ChannelA, LV_PART_MAIN);
    }

}