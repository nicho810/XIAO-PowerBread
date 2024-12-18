#include "lvgl_ui_updateFunc.h"

void update_chart_data(lv_obj_t *chart_container, int32_t new_value)
{
    if (!chart_container) {
        Serial.println("Error: chart_container is NULL");
        return;
    }

    // Get the chart object directly (it's the only child of the container)
    lv_obj_t *chart = lv_obj_get_child(chart_container, 0);
    if (!chart || !lv_obj_check_type(chart, &lv_chart_class)) {
        Serial.println("Error: Chart not found in container");
        return;
    }

    // Get the first series
    lv_chart_series_t *series = lv_chart_get_series_next(chart, NULL);
    if (!series) {
        Serial.println("Error: No series found in chart");
        return;
    }
    
    lv_chart_set_next_value(chart, series, new_value);
    lv_chart_refresh(chart);

    // Serial.print("Successfully set chart data to: ");
    // Serial.println(new_value);
}

void update_monitor_data(lv_obj_t *monitor_container, uint8_t channel, DualChannelData newSensorData)
{
    if (!monitor_container) {
        Serial.println("Error: monitor_container is NULL");
        return;
    }

    // Find the value labels (they are the 3rd, 5th, and 7th children)
    lv_obj_t *voltage_label = lv_obj_get_child(monitor_container, 2);
    lv_obj_t *current_label = lv_obj_get_child(monitor_container, 4);
    lv_obj_t *power_label = lv_obj_get_child(monitor_container, 6);

    if (!voltage_label || !current_label || !power_label) {
        Serial.println("Error: Labels not found in monitor container");
        return;
    }

    // Format values with adaptive decimal places
    char voltage_str[8], current_str[8], power_str[8];
    float voltage, current, power;

    // Get the values based on channel
    if (channel == 0) {
        voltage = newSensorData.channel0.busVoltage;
        current = newSensorData.channel0.busCurrent;
        power = newSensorData.channel0.busPower;
    } else if (channel == 1) {
        voltage = newSensorData.channel1.busVoltage;
        current = newSensorData.channel1.busCurrent;
        power = newSensorData.channel1.busPower;
    }

    // Format each value with adaptive decimal places
    if (voltage >= 100)       snprintf(voltage_str, sizeof(voltage_str), "%.1f", voltage);
    else if (voltage >= 10)   snprintf(voltage_str, sizeof(voltage_str), "%.2f", voltage);
    else                      snprintf(voltage_str, sizeof(voltage_str), "%.3f", voltage);

    if (current >= 100)       snprintf(current_str, sizeof(current_str), "%.1f", current);
    else if (current >= 10)   snprintf(current_str, sizeof(current_str), "%.2f", current);
    else                      snprintf(current_str, sizeof(current_str), "%.3f", current);

    if (power >= 100)         snprintf(power_str, sizeof(power_str), "%.1f", power);
    else if (power >= 10)     snprintf(power_str, sizeof(power_str), "%.2f", power);
    else                      snprintf(power_str, sizeof(power_str), "%.3f", power);

    // Update the labels
    lv_label_set_text(voltage_label, voltage_str);
    lv_label_set_text(current_label, current_str);
    lv_label_set_text(power_label, power_str);

    //debug
    // Serial.printf("Update Monitor Data: Channel %d, Voltage: %s, Current: %s, Power: %s\n", channel, voltage_str, current_str, power_str);
}

void update_count_data(lv_obj_t *count_container, uint8_t channel, DualChannelData newSensorData)
{
    //todo
}