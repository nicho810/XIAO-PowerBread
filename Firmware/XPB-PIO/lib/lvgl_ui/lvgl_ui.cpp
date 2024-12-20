#include "lvgl_ui.h"
#include "xpb_color_palette.h"
#include "FreeRTOS.h"
#include "task.h"

// Add this helper function at the top of the file
void cleanupAndWait() {
    // Delete all objects from the screen
    lv_obj_clean(lv_scr_act());
    
    // Force a screen update
    lv_refr_now(NULL);
    
    // Small delay to ensure cleanup is complete
    vTaskDelay(pdMS_TO_TICKS(10));
}

// Common function to get container dimensions based on rotation
void getContainerDimensions(int rotation, uint16_t &width, uint16_t &height) {
    if (rotation % 2 == 0) {  // Portrait (0 or 2)
        width = 80;
        height = 160;
    } else {  // Landscape (1 or 3)
        width = 160;
        height = 80;
    }
}

lv_obj_t*  dataMonitor_initUI(int rotation)
{
    cleanupAndWait();
    
    uint8_t container_width, container_height;
    int8_t widget_x1, widget_y1, widget_x2, widget_y2;
    
    // Set dimensions based on rotation
    if (rotation % 2 == 0) {  // Portrait
        container_width = 80;
        container_height = 160;
        widget_x1 = 0;
        widget_y1 = 41;
        widget_x2 = 0;
        widget_y2 = -41;
    } else {  // Landscape
        container_width = 160;
        container_height = 80;
        widget_x1 = -41;
        widget_y1 = 0;
        widget_x2 = 41;
        widget_y2 = 0;
    }

    // Create and configure container with updated dimensions
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_align(ui_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Create widgets with updated positions
    lv_obj_t *dataMonitor_A = widget_DataMonitor_create(widget_x1, widget_y1, "Channel A", xpb_color_ChannelA);
    lv_obj_t *dataMonitor_B = widget_DataMonitor_create(widget_x2, widget_y2, "Channel B", xpb_color_ChannelB);

    // Set parent to ui_container
    lv_obj_set_parent(dataMonitor_A, ui_container);
    lv_obj_set_parent(dataMonitor_B, ui_container);

    return ui_container;
}

lv_obj_t*  dataMonitorCount_initUI(int rotation, uint8_t channel)
{
    cleanupAndWait();
    
    uint16_t container_width, container_height;
    getContainerDimensions(rotation, container_width, container_height);

    // Create main container with correct dimensions
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Adjust positions based on rotation
    int monitor_y = (rotation % 2 == 0) ? -41 : 0;
    int base_y = (rotation % 2 == 0) ? 10 : -30;
    int x_offset = (rotation % 2 == 0) ? 0 : -20;
    int y_spacing = (rotation % 2 == 0) ? 20 : 15;

    // Create widgets with rotation-aware positioning
    if (channel == 0) {
        lv_obj_t *dataMonitor_X = widget_DataMonitor_create(0, monitor_y, "Channel A", xpb_color_ChannelA);
        lv_obj_t *dataCount_avgS = widget_DataCount_create(x_offset, base_y, "AvgS", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        lv_obj_t *dataCount_avgM = widget_DataCount_create(x_offset, base_y + y_spacing, "AvgM", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        lv_obj_t *dataCount_avgH = widget_DataCount_create(x_offset, base_y + y_spacing * 2, "AvgH", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        lv_obj_t *dataCount_peak = widget_DataCount_create(x_offset, base_y + y_spacing * 3, "Peak", xpb_color_ChannelA, xpb_color_ChannelA_dark);

        // Set parents
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataCount_avgS, ui_container);
        lv_obj_set_parent(dataCount_avgM, ui_container);
        lv_obj_set_parent(dataCount_avgH, ui_container);
        lv_obj_set_parent(dataCount_peak, ui_container);
    } else {
        // Similar setup for channel 1
        // ... existing channel 1 code ...
    }

    return ui_container;
}

lv_obj_t *dataMonitorChart_initUI(int rotation, uint8_t channel)
{
    cleanupAndWait();
    
    uint16_t container_width, container_height;
    getContainerDimensions(rotation, container_width, container_height);

    // Create main container with correct dimensions
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Adjust widget positions based on rotation
    int monitor_y = (rotation % 2 == 0) ? -41 : 0;
    int chart_y = (rotation % 2 == 0) ? 41 : 0;
    int monitor_x = (rotation % 2 == 0) ? 0 : -41;
    int chart_x = (rotation % 2 == 0) ? 0 : 41;

    // Create widgets with rotation-aware positioning
    if (channel == 0) {
        lv_obj_t *dataMonitor_X = widget_DataMonitor_create(monitor_x, monitor_y, "Channel A", xpb_color_ChannelA);
        lv_obj_t *dataChart_X = widget_DataChart_create(chart_x, chart_y, xpb_color_ChannelA, xpb_color_ChannelA_dark);
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataChart_X, ui_container);
    } else {
        // Similar setup for channel 1
        // ... existing channel 1 code ...
    }

    return ui_container;
}

lv_obj_t *widget_DataChart_create(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark)
{
    // Create a container
    lv_obj_t *container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN); // Black background
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);                // 2px border width
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);            // Red border color

    // Create a chart
    lv_obj_t *chart = lv_chart_create(container);
    
    // Basic chart setup
    lv_obj_set_size(chart, 78, 78);                    // Set chart size slightly smaller than container
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, 0);        // Center in container
    
    // Add these lines to adjust padding
    lv_obj_set_style_pad_left(chart, 4, LV_PART_MAIN);     // Remove left padding
    lv_obj_set_style_pad_right(chart, 4, LV_PART_MAIN);    // Remove right padding
    lv_obj_set_style_pad_top(chart, 4, LV_PART_MAIN);      // Remove top padding
    lv_obj_set_style_pad_bottom(chart, 4, LV_PART_MAIN);   // Remove bottom padding

    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);      // Set as line chart
    lv_obj_set_style_border_width(chart, 1, LV_PART_MAIN); //border width
    lv_obj_set_style_border_color(chart, color, LV_PART_MAIN); //border color
    lv_obj_set_style_radius(chart, 4, LV_PART_MAIN);    // Set chart corner radius
    
    // Configure chart properties
    lv_chart_set_point_count(chart, 70);              // Number of data points
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    
    // Set Y axis range to auto-scale
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);  // Initial range

    lv_chart_set_div_line_count(chart, 6, 0);          // Grid line count
    lv_obj_set_style_line_color(chart, xpb_color_GridLines, LV_PART_MAIN); //grid line color
    
    // Add dash pattern to grid lines
    static const lv_style_prop_t props[] = {LV_STYLE_PROP_INV};
    static lv_style_t style_grid;
    lv_style_init(&style_grid);
    lv_style_set_line_dash_width(&style_grid, 6);     // Length of dash
    lv_style_set_line_dash_gap(&style_grid, 4);       // Length of gap
    lv_obj_add_style(chart, &style_grid, LV_PART_MAIN);

    // Add and style a data series
    lv_chart_series_t *series = lv_chart_add_series(chart, color, LV_CHART_AXIS_PRIMARY_Y);
    
    // Enable area drawing mode for the series
    lv_chart_set_series_color(chart, series, color);

    // Style modifications
    lv_obj_set_style_line_width(chart, 1, LV_PART_ITEMS);    // Line thickness
    lv_obj_set_style_line_rounded(chart, true, LV_PART_ITEMS);  // Add this line to make the line curved
    lv_obj_set_style_bg_color(chart, xpb_color_Background, LV_PART_MAIN);
    // lv_obj_set_style_bg_opa(chart, LV_OPA_100, LV_PART_MAIN); // 100% opacity


    return container;
}

lv_obj_t *widget_DataCount_create(uint16_t x, uint16_t y, const char *title, lv_color_t color, lv_color_t color_dark)
{
    // Create a container
    lv_obj_t *container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(container, 78, 18);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN); // Black background
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);                // 2px border width
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);            // Red border color

    //create a title background shape in center-left corner
    lv_obj_t *title_background_1 = lv_obj_create(container);
    lv_obj_clear_flag(title_background_1, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(title_background_1, 30, 18);
    lv_obj_align(title_background_1, LV_ALIGN_LEFT_MID, -13, 0);
    lv_obj_set_style_radius(title_background_1, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title_background_1, color_dark, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_background_1, 0, LV_PART_MAIN); //no border

    // Create a title label in center-left corner
    lv_obj_t *title_label = lv_label_create(container);
    lv_obj_set_style_text_font(title_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, -10, -1);

    // Create a value label in center-left corner
    lv_obj_t *value_label = lv_label_create(container);
    lv_obj_set_style_text_font(value_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(value_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(value_label, "0.000");
    lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 20, 0);

    // Create a unit label in center-right corner
    lv_obj_t *unit_label = lv_label_create(container);
    lv_obj_set_style_text_font(unit_label, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(unit_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(unit_label, "mA");
    lv_obj_align(unit_label, LV_ALIGN_RIGHT_MID, 10, 0);

    return container;
}

lv_obj_t *widget_DataMonitor_create(uint16_t x, uint16_t y, const char *title_text, lv_color_t color)
{

    int pos_x_of_Voltage_lable = -10;
    int pos_y_of_Voltage_lable = -16;
    int pos_x_of_Voltage_unit = 10;
    int pos_y_of_Voltage_unit = pos_y_of_Voltage_lable + 3;

    int pos_x_of_Current_lable = pos_x_of_Voltage_lable;
    int pos_y_of_Current_lable = pos_y_of_Voltage_lable + 21;
    int pos_x_of_Current_unit = pos_x_of_Voltage_unit;
    int pos_y_of_Current_unit = pos_y_of_Current_lable + 3;

    int pos_x_of_Power_lable = pos_x_of_Voltage_lable;
    int pos_y_of_Power_lable = pos_y_of_Voltage_lable + 42;
    int pos_x_of_Power_unit = pos_x_of_Voltage_unit;
    int pos_y_of_Power_unit = pos_y_of_Power_lable + 3;

    // Create a container
    lv_obj_t *container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(container, 78, 78);
    lv_obj_align(container, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_radius(container, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, xpb_color_Background, LV_PART_MAIN); // Black background
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);                // 2px border width
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);            // Red border color

    // Create a title rectangle in top-left corner
    lv_obj_t *title_rect = lv_obj_create(container);
    lv_obj_clear_flag(title_rect, LV_OBJ_FLAG_SCROLLABLE); // Disable title rectangle scrolling
    lv_obj_set_size(title_rect, 50, 12);
    lv_obj_align(title_rect, LV_ALIGN_TOP_LEFT, -14, -14);
    lv_obj_set_style_radius(title_rect, 4, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title_rect, color, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_rect, 0, LV_PART_MAIN);

    // Create a title on the container
    lv_obj_t *title = lv_label_create(container);
    lv_obj_set_style_text_font(title, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(title, title_text);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, -10, -34);

    // Create a Voltage label on the container
    lv_obj_t *voltage_value = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(voltage_value, "0.000");
    lv_obj_align(voltage_value, LV_ALIGN_LEFT_MID, pos_x_of_Voltage_lable, pos_y_of_Voltage_lable);

    // Create a Voltage unit label on the container
    lv_obj_t *voltage_unit = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(voltage_unit, "V");
    lv_obj_align(voltage_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Voltage_unit, pos_y_of_Voltage_unit);

    // Create a Current label on the container
    lv_obj_t *current_value = lv_label_create(container);
    lv_obj_set_style_text_font(current_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(current_value, "0.000");
    lv_obj_align(current_value, LV_ALIGN_LEFT_MID, pos_x_of_Current_lable, pos_y_of_Current_lable);

    // Create a Current unit label on the container
    lv_obj_t *current_unit = lv_label_create(container);
    lv_obj_set_style_text_font(current_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(current_unit, "mA");
    lv_obj_align(current_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Current_unit, pos_y_of_Current_unit);

    // Create a Power label on the container
    lv_obj_t *power_value = lv_label_create(container);
    lv_obj_set_style_text_font(power_value, &inter_extraBold_18px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(power_value, "0.000");
    lv_obj_align(power_value, LV_ALIGN_LEFT_MID, pos_x_of_Power_lable, pos_y_of_Power_lable);

    // Create a Power unit label on the container
    lv_obj_t *power_unit = lv_label_create(container);
    lv_obj_set_style_text_font(power_unit, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(power_unit, "mW");
    lv_obj_align(power_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Power_unit, pos_y_of_Power_unit);

    return container;
}

