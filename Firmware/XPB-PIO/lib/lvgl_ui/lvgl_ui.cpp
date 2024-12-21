#include "lvgl_ui.h"

// Add this helper function at the top of the file
void cleanupAndWait() {
    lv_obj_clean(lv_scr_act()); // Delete all objects from the screen
    lv_refr_now(NULL);  // Force a screen update
    vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to ensure cleanup is complete
}

// Add this at the top of the file with other global variables
volatile bool menu_is_visible = false;

// Add event handling to the container
static void setup_container_events(lv_obj_t* container) {
    // Make container focusable
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    
    // Add to default group
    lv_group_t* g = lv_group_get_default();
    if (g) {
        lv_group_add_obj(g, container);
    }
    
    // Add key event handler
    lv_obj_add_event_cb(container, key_event_cb, LV_EVENT_KEY, NULL);
}

// Add this implementation
static void key_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* this_menu = NULL;
    if(code == LV_EVENT_KEY) {
        uint32_t key = lv_event_get_key(e);
        
        // Get the container (which is the target of the event)
        lv_obj_t* container = lv_event_get_target(e);
        
        switch(key) {
            case LV_KEY_UP:
                Serial.println("UP key pressed");
                break;
                
            case LV_KEY_DOWN:
                Serial.println("DOWN key pressed");
                break;
                
            case LV_KEY_ENTER:
                Serial.println("ENTER pressed(short press dial)");
                // Toggle menu visibility
                menu_is_visible = !menu_is_visible;
                
                // Find the menu object (last child of the container)
                if (lv_obj_get_child_cnt(container) > 0) {
                    this_menu = lv_obj_get_child(container, lv_obj_get_child_cnt(container) - 1);
                    
                    if (this_menu) {
                        if (menu_is_visible) {
                            lv_obj_clear_flag(this_menu, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(this_menu, LV_OBJ_FLAG_HIDDEN);
                        }
                    }
                }
                break;

            case LV_KEY_BACKSPACE:
                Serial.println("BACKSPACE pressed(long press dial)");
                break;
        }
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

    // Add event handling
    setup_container_events(ui_container);

    return ui_container;
}



lv_obj_t *dataMonitorChart_initUI(int rotation, uint8_t channel)
{
    cleanupAndWait();
    
    uint16_t container_width=80, container_height=160;

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

    // Add event handling
    setup_container_events(ui_container);

    return ui_container;
}

lv_obj_t*  dataMonitorCount_initUI(int rotation, uint8_t channel)
{
    cleanupAndWait();
    
    uint16_t container_width=80, container_height=160;

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
        lv_obj_t *dataMonitor_X = widget_DataMonitor_create(0, monitor_y, "Channel B", xpb_color_ChannelB);
        lv_obj_t *dataCount_avgS = widget_DataCount_create(x_offset, base_y, "AvgS", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        lv_obj_t *dataCount_avgM = widget_DataCount_create(x_offset, base_y + y_spacing, "AvgM", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        lv_obj_t *dataCount_avgH = widget_DataCount_create(x_offset, base_y + y_spacing * 2, "AvgH", xpb_color_ChannelB, xpb_color_ChannelB_dark);
        lv_obj_t *dataCount_peak = widget_DataCount_create(x_offset, base_y + y_spacing * 3, "Peak", xpb_color_ChannelB, xpb_color_ChannelB_dark);

        // Set parents
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataCount_avgS, ui_container);
        lv_obj_set_parent(dataCount_avgM, ui_container);
        lv_obj_set_parent(dataCount_avgH, ui_container);
        lv_obj_set_parent(dataCount_peak, ui_container);
    }

    // Create menu
    lv_obj_t *menu = widget_DataCount_Menu_create(0, 0, channel);
    lv_obj_set_parent(menu, ui_container);

    // Add event handling
    setup_container_events(ui_container);

    return ui_container;
}

