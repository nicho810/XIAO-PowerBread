#include "lvgl_ui.h"
#include "xpb_color_palette.h"


lv_obj_t*  dataMonitor_initUI(int rotation)
{
    // Create main container
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, 80, 160);  // Adjust size as needed
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Create widget
    lv_obj_t *dataMonitor_A = widget_DataMonitor_create(0, 41, "Channel A", xpb_color_ChannelA);
    lv_obj_t *dataMonitor_B = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);

    // Set parent to ui_container
    lv_obj_set_parent(dataMonitor_A, ui_container);
    lv_obj_set_parent(dataMonitor_B, ui_container);

    return ui_container;
}

lv_obj_t*  dataMonitorCount_initUI(int rotation, uint8_t channel)
{
    // Create main container
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, 80, 160);  // Adjust size as needed
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    //create widgets
    lv_obj_t *dataMonitor_X;
    lv_obj_t *dataCount_avgS;
    lv_obj_t *dataCount_avgM;
    lv_obj_t *dataCount_avgA;
    lv_obj_t *dataCount_peak;
    uint16_t pos_y_of_avgS = 10;
    if (channel == 0)
    {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel A", xpb_color_ChannelA);
        dataCount_avgS = widget_DataCount_create(0, pos_y_of_avgS, "AvgS", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        dataCount_avgM = widget_DataCount_create(0, pos_y_of_avgS + 20, "AvgM", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        dataCount_avgA = widget_DataCount_create(0, pos_y_of_avgS + 40, "AvgA", xpb_color_ChannelA, xpb_color_ChannelA_dark);
        dataCount_peak = widget_DataCount_create(0, pos_y_of_avgS + 60, "Peak", xpb_color_ChannelA, xpb_color_ChannelA_dark);
    }
    else if (channel == 1)
    {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);
        dataCount_avgS = widget_DataCount_create(0, pos_y_of_avgS, "AvgS", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        dataCount_avgM = widget_DataCount_create(0, pos_y_of_avgS + 20, "AvgM", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        dataCount_avgA = widget_DataCount_create(0, pos_y_of_avgS + 40, "AvgA", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        dataCount_peak = widget_DataCount_create(0, pos_y_of_avgS + 60, "Peak", xpb_color_ChannelB, xpb_color_ChannelB_dark);
    }

    //set parent to ui_container
    lv_obj_set_parent(dataMonitor_X, ui_container);
    lv_obj_set_parent(dataCount_avgS, ui_container);
    lv_obj_set_parent(dataCount_avgM, ui_container);
    lv_obj_set_parent(dataCount_avgA, ui_container);
    lv_obj_set_parent(dataCount_peak, ui_container);

    return ui_container;
}

lv_obj_t *dataMonitorChart_initUI(int rotation, uint8_t channel)
{
    // Create main container
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, 80, 160);  // Adjust size as needed
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Create widget
    lv_obj_t *dataChart_X = NULL;
    lv_obj_t *dataMonitor_X = NULL;
    if (channel == 0)
    {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel A", xpb_color_ChannelA);
        dataChart_X = widget_DataChart_create(0, 41, xpb_color_ChannelA, xpb_color_ChannelA_dark);
        // Set parent to ui_container
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataChart_X, ui_container);
    }
    else if (channel == 1)
    {
        dataMonitor_X = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);
        dataChart_X = widget_DataChart_create(0, 41, xpb_color_ChannelB, xpb_color_ChannelB_dark);
        // Set parent to ui_container
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataChart_X, ui_container);
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
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);      // Set as line chart
    lv_obj_set_style_border_width(chart, 1, LV_PART_MAIN); //border width
    lv_obj_set_style_border_color(chart, color, LV_PART_MAIN); //border color
    lv_obj_set_style_radius(chart, 4, LV_PART_MAIN);    // Set chart corner radius
    
    // Configure chart properties
    lv_chart_set_point_count(chart, 100);              // Number of data points
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 10);  // Y-axis range
    lv_chart_set_div_line_count(chart, 6, 0);          // Grid line count
    lv_obj_set_style_line_color(chart, xpb_color_GridLines, LV_PART_MAIN); //grid line color
    
    // Add and style a data series
    lv_chart_series_t *series = lv_chart_add_series(chart, color, LV_CHART_AXIS_PRIMARY_Y);
    
    // Style modifications
    lv_obj_set_style_line_width(chart, 2, LV_PART_ITEMS);    // Line thickness
    lv_obj_set_style_bg_color(chart, color_dark, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chart, LV_OPA_10, LV_PART_MAIN); // 10% opacity

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
    lv_obj_set_style_text_font(title_label, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(title_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, -10, -1);

    // Create a value label in center-left corner
    lv_obj_t *value_label = lv_label_create(container);
    lv_obj_set_style_text_font(value_label, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(value_label, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(value_label, "0.000");
    lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 20, 0);

    // Create a unit label in center-right corner
    lv_obj_t *unit_label = lv_label_create(container);
    lv_obj_set_style_text_font(unit_label, &inter_bold_8px_basicLatin, LV_PART_MAIN);
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
    lv_obj_set_style_text_font(title, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(title, title_text);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, -10, -34);

    // Create a Voltage label on the container
    lv_obj_t *voltage_value = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(voltage_value, "0.000");
    lv_obj_align(voltage_value, LV_ALIGN_LEFT_MID, pos_x_of_Voltage_lable, pos_y_of_Voltage_lable);

    // Create a Voltage unit label on the container
    lv_obj_t *voltage_unit = lv_label_create(container);
    lv_obj_set_style_text_font(voltage_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(voltage_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(voltage_unit, "V");
    lv_obj_align(voltage_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Voltage_unit, pos_y_of_Voltage_unit);

    // Create a Current label on the container
    lv_obj_t *current_value = lv_label_create(container);
    lv_obj_set_style_text_font(current_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(current_value, "0.000");
    lv_obj_align(current_value, LV_ALIGN_LEFT_MID, pos_x_of_Current_lable, pos_y_of_Current_lable);

    // Create a Current unit label on the container
    lv_obj_t *current_unit = lv_label_create(container);
    lv_obj_set_style_text_font(current_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(current_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(current_unit, "mA");
    lv_obj_align(current_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Current_unit, pos_y_of_Current_unit);

    // Create a Power label on the container
    lv_obj_t *power_value = lv_label_create(container);
    lv_obj_set_style_text_font(power_value, &inter_extraBold_18px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_value, xpb_color_Text, LV_PART_MAIN); // White tex
    lv_label_set_text(power_value, "0.000");
    lv_obj_align(power_value, LV_ALIGN_LEFT_MID, pos_x_of_Power_lable, pos_y_of_Power_lable);

    // Create a Power unit label on the container
    lv_obj_t *power_unit = lv_label_create(container);
    lv_obj_set_style_text_font(power_unit, &inter_bold_8px_basicLatin, LV_PART_MAIN);
    lv_obj_set_style_text_color(power_unit, xpb_color_Text, LV_PART_MAIN);
    lv_label_set_text(power_unit, "mW");
    lv_obj_align(power_unit, LV_ALIGN_RIGHT_MID, pos_x_of_Power_unit, pos_y_of_Power_unit);

    return container;
}

