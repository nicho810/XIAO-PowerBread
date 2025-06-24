#include "lvgl_ui.h"


UI_manager::UI_manager()
{
    current_UI_mode = UI_Mode_DataMonitor; //set default mode
    dataMonitor_A = nullptr;
    dataMonitor_B = nullptr;
}

UI_manager::~UI_manager()
{
    if (dataMonitor_A) {
        delete dataMonitor_A;
        dataMonitor_A = nullptr;
    }
    if (dataMonitor_B) {
        delete dataMonitor_B;
        dataMonitor_B = nullptr;
    }
}

void UI_manager::initUI(UI_Mode mode, lv_obj_t* container)
{   
    // If container is not provided, use the current screen
    if (container == NULL) container = lv_scr_act();

    current_UI_mode = mode;
    
    // Take LVGL mutex for thread safety
    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        Serial.println("ERROR: Failed to take LVGL mutex for UI init!");
        return;
    }

    // Clean the screen
    lv_obj_clean(container);
    
    // Create base container
    lv_obj_t* base_container = lv_obj_create(container);
    if (!base_container || !lv_obj_is_valid(base_container)) {
        Serial.println("ERROR: Failed to create base container!");
        xSemaphoreGive(lvglMutex);
        return;
    }
    
    // Set basic container properties
    lv_obj_set_size(base_container, screen_width, screen_height); // Standard screen size
    lv_obj_center(base_container);
    lv_obj_clear_flag(base_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(base_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_radius(base_container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(base_container, 0, LV_PART_MAIN);

    // Add event handling to the base container
    setup_container_events(base_container);

    // Initialize UI based on mode
    switch (mode) {
        case UI_Mode_Config:
            break;
            
        case UI_Mode_DataMonitor:
            Serial.println("> Initializing Data Monitor UI");
            // Create new instances and store them as member variables
            if (dataMonitor_A) {
                delete dataMonitor_A;
                dataMonitor_A = nullptr;
            }
            if (dataMonitor_B) {
                delete dataMonitor_B;
                dataMonitor_B = nullptr;
            }
            
            // Create widgets with proper parent container and error checking
            try {
                dataMonitor_A = new Widget_DataMonitor(0, 41, "Channel A", xpb_color_ChannelA, base_container);
                if (!dataMonitor_A) {
                    Serial.println("ERROR: Failed to create Channel A monitor!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                dataMonitor_B = new Widget_DataMonitor(0, -41, "Channel B", xpb_color_ChannelB, base_container);
                if (!dataMonitor_B) {
                    Serial.println("ERROR: Failed to create Channel B monitor!");
                    delete dataMonitor_A;
                    dataMonitor_A = nullptr;
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                Serial.println("> Data Monitor widgets created successfully");
            } catch (...) {
                Serial.println("ERROR: Exception during widget creation!");
                if (dataMonitor_A) {
                    delete dataMonitor_A;
                    dataMonitor_A = nullptr;
                }
                if (dataMonitor_B) {
                    delete dataMonitor_B;
                    dataMonitor_B = nullptr;
                }
                xSemaphoreGive(lvglMutex);
                return;
            }
            break;
            
        case UI_Mode_DataChart:
            break;
            
        case UI_Mode_DataCount:
            break;
            
        default:
            Serial.println("ERROR: Invalid UI mode!");
            break;
    }
    
    // Force refresh
    lv_disp_t *disp = lv_disp_get_default();
    if (disp) {
        lv_refr_now(disp);
    }
    
    xSemaphoreGive(lvglMutex);
}

void UI_manager::updateUI(UI_Mode mode, SensorDataMessage sensorDataMessage, lv_obj_t* container)
{
    // If container is not provided, use the current screen
    if (container == NULL) container = lv_scr_act();

    // If the mode is not the current mode, return
    if (mode != current_UI_mode) {
        return;
    }
    
    // Check available heap memory
    size_t freeHeap = ESP.getFreeHeap();
    if (freeHeap < 10000) { // Less than 10KB free
        Serial.printf("WARNING: Low heap memory: %d bytes\n", freeHeap);
    }
    
    // Take LVGL mutex for thread safety
    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return; // Don't block if mutex is busy
    }
    
    // Get the main container
    if (!container || !lv_obj_is_valid(container)) {
        xSemaphoreGive(lvglMutex);
        return;
    }
    
    // Update UI based on mode
    switch (mode) {
        case UI_Mode_Config:
            // Config mode updates are handled by the config mode system
            break;
            
        case UI_Mode_DataMonitor:
            // Update monitor data for both channels with proper unit handling
            if (dataMonitor_A && dataMonitor_B) {
                // Convert mV to V for voltage display
                float voltage_A = sensorDataMessage.data[0].busVoltage_mV / 1000.0f;
                float voltage_B = sensorDataMessage.data[1].busVoltage_mV / 1000.0f;
                
                // Keep current in mA (no conversion needed)
                float current_A = sensorDataMessage.data[0].current_mA;
                float current_B = sensorDataMessage.data[1].current_mA;
                
                // Keep power in mW (no conversion needed)
                float power_A = sensorDataMessage.data[0].power_mW;
                float power_B = sensorDataMessage.data[1].power_mW;
                
                // Update with null checks and error handling
                if (dataMonitor_A->getContainer() && lv_obj_is_valid(dataMonitor_A->getContainer())) {
                    dataMonitor_A->setVoltage(voltage_A);
                    dataMonitor_A->setCurrent(current_A);
                    dataMonitor_A->setPower(power_A);
                }
                
                if (dataMonitor_B->getContainer() && lv_obj_is_valid(dataMonitor_B->getContainer())) {
                    dataMonitor_B->setVoltage(voltage_B);
                    dataMonitor_B->setCurrent(current_B);
                    dataMonitor_B->setPower(power_B);
                }
            }
            break;
            
        case UI_Mode_DataChart:
            // Update chart data for current channel
            // update_chart_data(container, highLightChannel, latestSensorData, 0.0f, 0);
            break;
            
        case UI_Mode_DataCount:
            // Update count data for current channel
            // update_count_data(container, highLightChannel, latestSensorData, 0.0f, 0);
            break;
            
        default:
            break;
    }
    
    xSemaphoreGive(lvglMutex);
}

void UI_manager::switch_UI(UI_Mode mode)
{
    if (mode != current_UI_mode) {
        Serial.printf("> Switching UI from mode %d to mode %d\n", current_UI_mode, mode);
        initUI(mode);
    }
}
/*
void dataMonitor_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    // Guard against reentrant calls
    if (ui_initialization_in_progress) {
        Serial.println("WARNING: UI initialization already in progress, skipping");
        return;
    }
    ui_initialization_in_progress = true;

    Serial.println("Starting dataMonitor_initUI");
    Serial.flush();

    if (!ui_container || !lv_obj_is_valid(ui_container)) {
        Serial.println("ERROR: Invalid container in dataMonitor_initUI!");
        ui_initialization_in_progress = false;
        return;
    }

    // Store monitors as static to prevent deallocation
    static lv_obj_t *dataMonitor_A = NULL;
    static lv_obj_t *dataMonitor_B = NULL;

    // Clear existing objects but keep the container
    lv_obj_clean(ui_container);
    
    Serial.println("Creating Channel A monitor");
    Serial.flush();

    // Create Channel A monitor with error checking
    dataMonitor_A = widget_DataMonitor_create(0, 41, "Channel A", xpb_color_ChannelA);
    if (!dataMonitor_A || !lv_obj_is_valid(dataMonitor_A)) {
        Serial.println("ERROR: Failed to create Channel A monitor!");
        ui_initialization_in_progress = false;
        return;
    }
    
    Serial.println("Creating Channel B monitor");
    Serial.flush();

    // Create Channel B monitor with error checking
    dataMonitor_B = widget_DataMonitor_create(0, -41, "Channel B", xpb_color_ChannelB);
    if (!dataMonitor_B || !lv_obj_is_valid(dataMonitor_B)) {
        Serial.println("ERROR: Failed to create Channel B monitor!");
        ui_initialization_in_progress = false;
        return;
    }

    // Set parents with validation
    if (lv_obj_is_valid(ui_container) && lv_obj_is_valid(dataMonitor_A)) {
        lv_obj_set_parent(dataMonitor_A, ui_container);
        Serial.println("Channel A monitor parented");
        Serial.flush();
    }

    if (lv_obj_is_valid(ui_container) && lv_obj_is_valid(dataMonitor_B)) {
        lv_obj_set_parent(dataMonitor_B, ui_container);
        Serial.println("Channel B monitor parented");
        Serial.flush();
    }

    // Force a refresh to ensure everything is displayed
    lv_disp_t *disp = lv_disp_get_default();
    if (disp) {
        lv_refr_now(disp);
    }

    Serial.println("DataMonitor UI initialization complete");
    Serial.flush();
    ui_initialization_in_progress = false;
}

void dataMonitorChart_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    cleanupAndWait();

    uint16_t container_width = 80, container_height = 160;

    // Create main container with correct dimensions
    // lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Adjust widget positions based on rotation
    int rotation = tft_Rotation;
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

    // return ui_container;
}

void dataMonitorCount_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue)
{
    cleanupAndWait();

    uint16_t container_width = 80, container_height = 160;

    // Create main container with correct dimensions
    // lv_obj_t *ui_container = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_container, container_width, container_height);
    lv_obj_center(ui_container);
    lv_obj_set_style_bg_color(ui_container, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN); // no border

    // Adjust positions based on rotation
    int rotation = tft_Rotation;
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

    // return ui_container;
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
    lv_obj_set_size(item_area, 80, 148);
    lv_obj_set_style_pad_top(item_area, 16, LV_PART_MAIN);    
    lv_obj_set_style_pad_bottom(item_area, 0, LV_PART_MAIN); 
    lv_obj_set_style_pad_left(item_area, 0, LV_PART_MAIN);   // Set left padding to 0
    lv_obj_set_scroll_dir(item_area, LV_DIR_VER);            
    lv_obj_align(item_area, LV_ALIGN_TOP_MID, 0, -2);
    lv_obj_set_style_bg_color(item_area, xpb_color_Background, LV_PART_MAIN);
    lv_obj_set_style_border_width(item_area, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(item_area, xpb_color_ChannelA, LV_PART_MAIN);
    lv_obj_set_style_radius(item_area, 0, LV_PART_MAIN);

    // Add items
    uint8_t item_spacing_y = 16;
    int8_t item_firstItemPos_y = -10;
    // int16_t total_content_height = item_firstItemPos_y + (item_spacing_y * 4);  // 11 items
    // lv_obj_set_content_height(item_area, total_content_height);

    
    // Create instance of sysConfig_cfgName to access config names
    sysConfig_cfgName configNames;
    
    // Create items using config names from sysConfig - removed the -14 x offset
    lv_obj_t *item_0 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 0, configNames.items[CFG_DEFAULT_MODE].name, sysConfig.cfg_data.default_mode, 1);
    lv_obj_t *item_1 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 1, configNames.items[CFG_DEFAULT_CHANNEL].name, sysConfig.cfg_data.default_channel, 0);
    lv_obj_t *item_2 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 2, configNames.items[CFG_SHUNT_RESISTOR_CHA].name, sysConfig.cfg_data.shuntResistorCHA, 0);
    lv_obj_t *item_3 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 3, configNames.items[CFG_SHUNT_RESISTOR_CHB].name, sysConfig.cfg_data.shuntResistorCHB, 0);
    lv_obj_t *item_4 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 4, configNames.items[CFG_SERIAL_ENABLE].name, sysConfig.cfg_data.serial_enable, 0);
    lv_obj_t *item_5 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 5, configNames.items[CFG_SERIAL_BAUDRATE].name, sysConfig.cfg_data.serial_baudRate, 0);
    lv_obj_t *item_6 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 6, configNames.items[CFG_SERIAL_MODE].name, sysConfig.cfg_data.serial_mode, 0);
    lv_obj_t *item_7 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 7, configNames.items[CFG_SERIAL_PRINT_INTERVAL].name, sysConfig.cfg_data.serial_printInterval, 0);
    lv_obj_t *item_8 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 8, configNames.items[CFG_CHART_UPDATE_INTERVAL].name, sysConfig.cfg_data.chart_updateInterval, 0);
    lv_obj_t *item_9 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 9, configNames.items[CFG_CHART_SCALE_MODE].name, sysConfig.cfg_data.chart_scaleMode, 0);
    lv_obj_t *item_10 = widget_configMode_item(item_area, 0, item_firstItemPos_y + item_spacing_y * 10, configNames.items[CFG_CHART_SCALE].name, sysConfig.cfg_data.chart_scale, 0);

    return ui_container;
}

*/