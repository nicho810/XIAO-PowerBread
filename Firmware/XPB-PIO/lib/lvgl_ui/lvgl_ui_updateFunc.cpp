#include "lvgl_ui_updateFunc.h"

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
    float voltage, current, power;

    // Direct value assignment without branching
    voltage = (channel == 0) ? newSensorData.channel0.busVoltage : newSensorData.channel1.busVoltage;
    current = (channel == 0) ? newSensorData.channel0.busCurrent : newSensorData.channel1.busCurrent;
    power = (channel == 0) ? newSensorData.channel0.busPower : newSensorData.channel1.busPower;

    // Get all labels at once
    lv_obj_t *voltage_label = lv_obj_get_child(monitor_container, 2);
    lv_obj_t *current_label = lv_obj_get_child(monitor_container, 4);
    lv_obj_t *power_label = lv_obj_get_child(monitor_container, 6);

    if (!voltage_label || !current_label || !power_label) return;

    // Format and update voltage (fixed 3 decimal places)
    snprintf(str_buf, sizeof(str_buf), "%0.3f", voltage);
    lv_label_set_text(voltage_label, str_buf);

    // Format and update current (fixed 3 decimal places)
    snprintf(str_buf, sizeof(str_buf), "%0.3f", current);
    lv_label_set_text(current_label, str_buf);

    // Format and update power (fixed 3 decimal places)
    snprintf(str_buf, sizeof(str_buf), "%0.3f", power);
    lv_label_set_text(power_label, str_buf);
}

void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData)
{
    // Implementation will be added when needed
    (void)count_container;
    (void)channel;
    (void)newSensorData;
}