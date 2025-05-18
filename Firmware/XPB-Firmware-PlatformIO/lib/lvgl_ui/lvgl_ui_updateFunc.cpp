#include "lvgl_ui_updateFunc.h"
#include "currentSensor.h"
#include "sysConfig.h"

extern sysConfig_data tmp_cfg_data;

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

void update_chart_data(lv_obj_t *chart_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    if (!chart_container) return;

    int32_t new_value = intValue;
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

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    if (!monitor_container) return;

    static char str_buf[8];  // Single buffer for all string conversions
    
    const currentSensorData* data = channel ? &newSensorData.channel1 : &newSensorData.channel0;

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

void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    if (!count_container) return;

    static char str_buf[8];  // Single buffer for all string conversions

    // Get value label - it's the second child (index 1)
    lv_obj_t* value_label = lv_obj_get_child(count_container, 2);
    if (!value_label) return;
    
    format_value(str_buf, sizeof(str_buf), floatValue);
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


void update_configMode(lv_obj_t *item_area, int8_t cursor, int8_t cursor_last, int8_t cursor_max, int8_t cursor_status)
{
    if (!item_area) return;

    // Validate cursor ranges
    if (cursor < 0 || cursor >= cursor_max || cursor_last < -1 || cursor_last >= cursor_max) return;

    // Clear previous highlight if valid
    if (cursor_last >= 0) {
        lv_obj_t *last_item = lv_obj_get_child(item_area, cursor_last);
        if (last_item) {
            lv_obj_t *last_value_box = lv_obj_get_child(last_item, 1);
            if (last_value_box) {
                update_configMode_changeItemStatus(last_value_box, 0);
            }
        }
    }

    // Update current item
    lv_obj_t *current_item = lv_obj_get_child(item_area, cursor);
    if (current_item) {
        lv_obj_t *current_value_box = lv_obj_get_child(current_item, 1);
        if (current_value_box) {
            update_configMode_changeItemStatus(current_value_box, 
                cursor_status == 0 ? 1 : 2); // 1 for hover, 2 for selected
        }

        // Scroll to make the current item visible
        lv_coord_t item_y = lv_obj_get_y(current_item);
        lv_coord_t container_height = lv_obj_get_height(item_area);
        lv_coord_t item_height = lv_obj_get_height(current_item);
        
        // Calculate the visible area boundaries
        lv_coord_t scroll_top = lv_obj_get_scroll_y(item_area);
        lv_coord_t visible_top = scroll_top;
        lv_coord_t visible_bottom = scroll_top + container_height;
        
        // If item is above visible area
        if (item_y < visible_top + 30) { // Increase offset for more scrolling
            lv_obj_scroll_to_y(item_area, item_y - 30, LV_ANIM_ON);
        }
        // If item is below visible area
        else if ((item_y + item_height) > visible_bottom - 30) {
            lv_obj_scroll_to_y(item_area, item_y - container_height + item_height + 30, LV_ANIM_ON);
        }
    }
}

void update_configMode_changeItemStatus(lv_obj_t *item, int8_t itemStatus){
    if (!item) return;

    if (itemStatus == 0) { // normal(not selected)
        lv_obj_set_style_border_width(item, 0, LV_PART_MAIN);
    } else if (itemStatus == 1) { // Hover
        lv_obj_set_style_border_width(item, 1, LV_PART_MAIN);
        lv_obj_set_style_border_color(item, xpb_color_Text, LV_PART_MAIN);
    } else if (itemStatus == 2) { // Selected
        lv_obj_set_style_border_width(item, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item, xpb_color_ChannelA, LV_PART_MAIN);
    }

}

void update_configMode_cfgData(lv_obj_t *item_area, int8_t cursor) {
    if (!item_area) return;

    lv_obj_t *current_item = lv_obj_get_child(item_area, cursor);
    if (current_item) {
        lv_obj_t *current_value_box = lv_obj_get_child(current_item, 1);
        lv_obj_t *current_value_label = lv_obj_get_child(current_value_box, 0); // get the value label
        if (current_value_label) {
            uint8_t value = 0;
            //get value from tmp_cfg_data by cursor

            switch (cursor) {
                case 0: value = tmp_cfg_data.default_mode; break;
                case 1: value = tmp_cfg_data.default_channel; break;
                case 2: value = tmp_cfg_data.shuntResistorCHA; break;
                case 3: value = tmp_cfg_data.shuntResistorCHB; break;
                case 4: value = tmp_cfg_data.serial_enable; break;
                case 5: value = tmp_cfg_data.serial_baudRate; break;
                case 6: value = tmp_cfg_data.serial_mode; break;
                case 7: value = tmp_cfg_data.serial_printInterval; break;
                case 8: value = tmp_cfg_data.chart_updateInterval; break;
                case 9: value = tmp_cfg_data.chart_scaleMode; break;
                case 10: value = tmp_cfg_data.chart_scale; break;
            }

            char value_str[4];
            snprintf(value_str, sizeof(value_str), "%u", value);
            lv_label_set_text(current_value_label, value_str);
        }
    }
}