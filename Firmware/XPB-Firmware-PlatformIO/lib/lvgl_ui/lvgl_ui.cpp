#include "lvgl_ui.h"
#include "sysConfig.h"

extern ConfigMode configMode;
// extern volatile bool is_configMode_active;
// extern volatile int8_t configMode_cursor;
// extern volatile int8_t configMode_cursor_last; // last cursor position
// extern volatile int8_t configMode_cursor_status;
// extern volatile uint8_t configMode_cursor_max;

// Add this helper function at the top of the file
void cleanupAndWait()
{
    lv_obj_clean(lv_scr_act());    // Delete all objects from the screen
    lv_refr_now(NULL);             // Force a screen update
    vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to ensure cleanup is complete
}

// Add this at the top of the file with other global variables
volatile bool menu_is_visible = false;

// Add at the top with other global variables
static uint32_t last_key_time = 0;
const uint32_t KEY_DEBOUNCE_MS = 50;  // Minimum time between key events

// Add event handling to the container
static void setup_container_events(lv_obj_t *container)
{
    // Make container focusable
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    // Add to default group
    lv_group_t *g = lv_group_get_default();
    if (g)
    {
        lv_group_add_obj(g, container);
    }
    // Add key event handler
    lv_obj_add_event_cb(container, key_event_cb, LV_EVENT_KEY, NULL);
}

// Add this implementation
static void key_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_KEY)
    {
        // Rate limiting check
        uint32_t current_time = millis();
        if (current_time - last_key_time < KEY_DEBOUNCE_MS) {
            return;  // Ignore rapid keypresses
        }
        last_key_time = current_time;

        uint32_t key = lv_event_get_key(e);

        if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(50)) == pdTRUE)  // Increased timeout
        {
            ConfigModeState currentState;
            if (configMode.getConfigState(&currentState))  // Safely get current state
            {
                if (currentState.isActive)
                {
                    int8_t newCursor = currentState.cursor;  // Work with local copy
                    bool stateChanged = false;

                    switch (key)
                    {
                    case LV_KEY_UP:
                        if (newCursor > 0 && newCursor < currentState.cursorMax) {  // Extra bounds validation
                            newCursor--;
                            stateChanged = true;
                        }
                        break;

                    case LV_KEY_DOWN:
                        if (newCursor >= 0 && newCursor < (currentState.cursorMax - 1)) {  // Extra bounds validation
                            newCursor++;
                            stateChanged = true;
                        }
                        break;

                    case LV_KEY_ENTER:
                        currentState.cursorStatus = 1; //1 is max, it means select the item, and it can edit the value
                        stateChanged = true;
                        break;

                    case LV_KEY_ESC:
                        currentState.cursorStatus--;
                        if (currentState.cursorStatus < -1) { //-1 is min, it means exit the config mode, 0 is normal(not selected)
                            currentState.cursorStatus = -1;
                        }
                        stateChanged = true;
                        break;
                    }

                    if (stateChanged && 
                        newCursor >= 0 && 
                        newCursor < currentState.cursorMax) {  // Final bounds check
                        currentState.cursorLast = currentState.cursor;
                        currentState.cursor = newCursor;
                        if (!configMode.updateConfigState(&currentState)) {  // Check if update succeeded
                            Serial.println("Failed to update config state");
                        }
                    }

                    // Debug output
                    Serial.printf("Config mode: cursor=%d, last=%d, max=%d, status=%d\n", 
                        currentState.cursor, 
                        currentState.cursorLast,
                        currentState.cursorMax,
                        currentState.cursorStatus);
                }
                else // not in config mode
                {
                    switch (key)
                    {
                    case LV_KEY_UP: // Dial turned up (status 1)
                        Serial.println("UP key pressed");
                        if (current_functionMode > dataMonitor)
                        {
                            current_functionMode = static_cast<function_mode>(current_functionMode - 1);
                        }
                        else
                        {
                            current_functionMode = dataMonitorCount;
                        }
                        functionMode_ChangeRequested = true;
                        break;

                    case LV_KEY_DOWN: // Dial turned down (status 2)
                        Serial.println("DOWN key pressed");
                        if (current_functionMode < dataMonitorCount)
                        {
                            current_functionMode = static_cast<function_mode>(current_functionMode + 1);
                        }
                        else
                        {
                            current_functionMode = dataMonitor;
                        }
                        functionMode_ChangeRequested = true;
                        break;

                    case LV_KEY_ENTER: // Short press (status 3)
                        Serial.println("ENTER key pressed");
                        highLightChannel = (highLightChannel + 1) % 2;
                        highLightChannel_ChangeRequested = true;

                        if (current_functionMode == dataMonitorChart ||
                            current_functionMode == dataMonitorCount)
                        {
                            functionMode_ChangeRequested = true;
                        }
                        break;

                    case LV_KEY_ESC: // Long press (status 4)
                        Serial.println("ESC pressed(long press dial)");
                        // Add any long press handling here
                        break;
                    }
                }
            }
            xSemaphoreGive(xSemaphore);
        }
        else {
            Serial.println("Failed to take semaphore in key_event_cb");
        }
    }
}

lv_obj_t *dataMonitor_initUI(int rotation)
{
    cleanupAndWait();

    uint8_t container_width, container_height;
    int8_t widget_x1, widget_y1, widget_x2, widget_y2;

    // Set dimensions based on rotation
    if (rotation % 2 == 0)
    { // Portrait
        container_width = 80;
        container_height = 160;
        widget_x1 = 0;
        widget_y1 = 41;
        widget_x2 = 0;
        widget_y2 = -41;
    }
    else
    { // Landscape
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

    uint16_t container_width = 80, container_height = 160;

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
    if (channel == 0)
    {
        lv_obj_t *dataMonitor_X = widget_DataMonitor_create(monitor_x, monitor_y, "Channel A", xpb_color_ChannelA);
        lv_obj_t *dataChart_X = widget_DataChart_create(chart_x, chart_y, xpb_color_ChannelA, xpb_color_ChannelA_dark);
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataChart_X, ui_container);
    }
    else
    {
        lv_obj_t *dataMonitor_X = widget_DataMonitor_create(monitor_x, monitor_y, "Channel B", xpb_color_ChannelB);
        lv_obj_t *dataChart_X = widget_DataChart_create(chart_x, chart_y, xpb_color_ChannelB, xpb_color_ChannelB_dark);
        lv_obj_set_parent(dataMonitor_X, ui_container);
        lv_obj_set_parent(dataChart_X, ui_container);
    }

    // Add event handling
    setup_container_events(ui_container);

    return ui_container;
}

lv_obj_t *dataMonitorCount_initUI(int rotation, uint8_t channel)
{
    cleanupAndWait();

    uint16_t container_width = 80, container_height = 160;

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
    if (channel == 0)
    {
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
    }
    else
    {
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

    // // Create menu
    // lv_obj_t *menu = widget_DataCount_Menu_create(0, 0, channel);
    // lv_obj_set_parent(menu, ui_container);

    // Add event handling
    setup_container_events(ui_container);

    return ui_container;
}

lv_obj_t *configMode_initUI(int rotation)
{
    cleanupAndWait();

    uint16_t container_width = 80, container_height = 160;

    // Create main container
    lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN);

    // Add event handling to the container BEFORE creating child objects
    setup_container_events(ui_container);

    // Make the container focusable and focused by default
    lv_obj_add_flag(ui_container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui_container, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_group_t *g = lv_group_get_default();
    if (g)
    {
        lv_group_add_obj(g, ui_container);
        lv_group_focus_obj(ui_container); // Explicitly focus the container
    }

    // title background
    lv_obj_t *title_bg = lv_obj_create(ui_container);
    lv_obj_clear_flag(title_bg, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(title_bg, 80, 12);
    lv_obj_align(title_bg, LV_ALIGN_TOP_MID, 0, -12);
    lv_obj_set_style_bg_color(title_bg, xpb_color_ChannelA, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_bg, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(title_bg, 0, LV_PART_MAIN);

    // title text
    lv_obj_t *title_text = lv_label_create(title_bg);
    lv_label_set_text(title_text, "CONFIG MODE");
    lv_obj_set_style_text_color(title_text, xpb_color_Text, LV_PART_MAIN);
    lv_obj_set_style_text_font(title_text, &inter_bold_8px_basicLatin_2b, LV_PART_MAIN);
    lv_obj_align(title_text, LV_ALIGN_CENTER, 0, -1);

    // Item area
    lv_obj_t *item_area = lv_obj_create(ui_container);
    lv_obj_clear_flag(item_area, LV_OBJ_FLAG_SCROLLABLE); // Disable container scrolling
    lv_obj_set_size(item_area, 80, 148);
    lv_obj_align(item_area, LV_ALIGN_BOTTOM_MID, 0, 12);
    lv_obj_set_style_bg_color(item_area, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(item_area, 0, LV_PART_MAIN);
    lv_obj_set_style_border_color(item_area, xpb_color_ChannelA, LV_PART_MAIN);
    lv_obj_set_style_radius(item_area, 0, LV_PART_MAIN);

    // Add items
    uint8_t item_spacing_y = 16;
    int8_t item_firstItemPos_y = -10;
    lv_obj_t *item_1 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 0, "Unit 1", 16, 1);
    lv_obj_t *item_2 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 1, "Unit 2", 255, 0);
    lv_obj_t *item_3 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 2, "Unit 3", 128, 0);
    lv_obj_t *item_4 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 3, "Unit 4", 128, 0);
    lv_obj_t *item_5 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 4, "Unit 5", 128, 0);
    lv_obj_t *item_6 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 5, "Unit 6", 128, 0);
    lv_obj_t *item_7 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 6, "Unit 7", 128, 0);
    lv_obj_t *item_8 = widget_configMode_item(item_area, -12, item_firstItemPos_y + item_spacing_y * 7, "Unit 8", 128, 0);


    return ui_container;
}
