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

void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData)
{
    // Implementation will be added when needed
    (void)count_container;
    (void)channel;
    (void)newSensorData;
}