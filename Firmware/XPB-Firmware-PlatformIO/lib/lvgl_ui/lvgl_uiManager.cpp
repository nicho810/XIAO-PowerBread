#include "lvgl_uiManager.h"

extern SemaphoreHandle_t lvglMutex;



UI_manager::UI_manager()
{
    current_UI_mode = UI_Mode_DataMonitor; //set default mode
    dataMonitor_1 = nullptr;
    dataMonitor_2 = nullptr;
    dataChart_1 = nullptr;
    dataChart_2 = nullptr;
    dataCount_1 = nullptr;
    dataCount_2 = nullptr;
}

UI_manager::~UI_manager()
{
    if (dataMonitor_1) {
        delete dataMonitor_1;
        dataMonitor_1 = nullptr;
    }
    if (dataMonitor_2) {
        delete dataMonitor_2;
        dataMonitor_2 = nullptr;
    }
    if (dataChart_1) {
        delete dataChart_1;
        dataChart_1 = nullptr;
    }
    if (dataChart_2) {
        delete dataChart_2;
        dataChart_2 = nullptr;
    }
    if (dataCount_1) {
        delete dataCount_1;
        dataCount_1 = nullptr;
    }
    if (dataCount_2) {
        delete dataCount_2;
        dataCount_2 = nullptr;
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
            if (dataMonitor_1) {
                delete dataMonitor_1;
                dataMonitor_1 = nullptr;
            }
            if (dataMonitor_2) {
                delete dataMonitor_2;
                dataMonitor_2 = nullptr;
            }
            
            // Create widgets with proper parent container and error checking
            try {
                dataMonitor_1 = new Widget_DataMonitor(0, 41, "Channel 1", xpb_color_ChannelA, base_container);
                if (!dataMonitor_1) {
                    Serial.println("ERROR: Failed to create Channel A monitor!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                dataMonitor_2 = new Widget_DataMonitor(0, -41, "Channel 2", xpb_color_ChannelB, base_container);
                if (!dataMonitor_2) {
                    Serial.println("ERROR: Failed to create Channel B monitor!");
                    delete dataMonitor_1;
                    dataMonitor_1 = nullptr;
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                Serial.println("> Data Monitor widgets created successfully");
            } catch (...) {
                Serial.println("ERROR: Exception during widget creation!");
                if (dataMonitor_1) {
                    delete dataMonitor_1;
                    dataMonitor_1 = nullptr;
                }
                if (dataMonitor_2) {
                    delete dataMonitor_2;
                    dataMonitor_2 = nullptr;
                }
                xSemaphoreGive(lvglMutex);
                return;
            }
            break;
            
        case UI_Mode_DataChart_1:
            Serial.println("> Initializing Data Chart UI #1");
            // Create new instances and store them as member variables
            if (dataMonitor_1) {
                delete dataMonitor_1;
                dataMonitor_1 = nullptr;
            }
            if (dataChart_1) {
                delete dataChart_1;
                dataChart_1 = nullptr;
            }
            // Create widgets with proper parent container and error checking
            try {
                dataMonitor_1 = new Widget_DataMonitor(0, -41, "Channel 1", xpb_color_ChannelA, base_container);
                if (!dataMonitor_1) {
                    Serial.println("ERROR: Failed to create Channel A monitor!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                dataChart_1 = new Widget_DataChart(0, 41, xpb_color_ChannelA, xpb_color_ChannelA_dark);
                if (!dataChart_1) {
                    Serial.println("ERROR: Failed to create Channel 1 chart!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                Serial.println("> Data Monitor and Chart widgets created successfully");
            } catch (...) {
                Serial.println("ERROR: Exception during widget creation!");
                if (dataMonitor_1) {
                    delete dataMonitor_1;
                    dataMonitor_1 = nullptr;
                }
                if (dataChart_1) {
                    delete dataChart_1;
                    dataChart_1 = nullptr;
                }
                xSemaphoreGive(lvglMutex);
                return;
            }
            break;

        case UI_Mode_DataChart_2:
            Serial.println("> Initializing Data Chart UI #2");
            // Create new instances and store them as member variables
            if (dataMonitor_2) {
                delete dataMonitor_2;
                dataMonitor_2 = nullptr;
            }
            if (dataChart_2) {
                delete dataChart_2;
                dataChart_2 = nullptr;
            }
            // Create widgets with proper parent container and error checking
            try {
                dataMonitor_2 = new Widget_DataMonitor(0, -41, "Channel 2", xpb_color_ChannelB, base_container);
                if (!dataMonitor_2) {
                    Serial.println("ERROR: Failed to create Channel B monitor!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                dataChart_2 = new Widget_DataChart(0, 41, xpb_color_ChannelB, xpb_color_ChannelB_dark);
                if (!dataChart_2) {
                    Serial.println("ERROR: Failed to create Channel 2 chart!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                Serial.println("> Data Monitor and Chart widgets created successfully");
            } catch (...) {
                Serial.println("ERROR: Exception during widget creation!");
                if (dataMonitor_2) {
                    delete dataMonitor_2;
                    dataMonitor_2 = nullptr;
                }
                if (dataChart_2) {
                    delete dataChart_2;
                    dataChart_2 = nullptr;
                }
                xSemaphoreGive(lvglMutex);
                return;
            }
            break;
        case UI_Mode_DataCount_1:
            Serial.println("> Initializing Data Count UI #1");
            // Create new instances and store them as member variables
            if (dataMonitor_1) {
                delete dataMonitor_1;
                dataMonitor_1 = nullptr;
            }
            if (dataCount_1) {
                delete dataCount_1;
                dataCount_1 = nullptr;
            }
            // Create widgets with proper parent container and error checking
            try {
                dataMonitor_1 = new Widget_DataMonitor(0, -41, "Channel 1", xpb_color_ChannelA, base_container);
                if (!dataMonitor_1) {
                    Serial.println("ERROR: Failed to create Channel A monitor!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                
                dataCount_1 = new Widget_DataCount(0, 41, xpb_color_ChannelA, xpb_color_ChannelA_dark, base_container);
                if (!dataCount_1) {
                    Serial.println("ERROR: Failed to create Channel 1 count!");
                    xSemaphoreGive(lvglMutex);
                    return;
                }
                Serial.println("> Data Monitor and Count widgets created successfully");
            } catch (...) {
                Serial.println("ERROR: Exception during widget creation!");
                if (dataMonitor_1) {
                    delete dataMonitor_1;
                    dataMonitor_1 = nullptr;
                }
                if (dataCount_1) {
                    delete dataCount_1;
                    dataCount_1 = nullptr;
                }
                xSemaphoreGive(lvglMutex);
                return;
            }
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

    // Set the previous mode
    setPreviousMode(mode);
    
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
            if (dataMonitor_1 && dataMonitor_2) {
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
                if (dataMonitor_1->getContainer() && lv_obj_is_valid(dataMonitor_1->getContainer())) {
                    dataMonitor_1->setVoltage(voltage_A);
                    dataMonitor_1->setCurrent(current_A);
                    dataMonitor_1->setPower(power_A);
                }
                
                if (dataMonitor_2->getContainer() && lv_obj_is_valid(dataMonitor_2->getContainer())) {
                    dataMonitor_2->setVoltage(voltage_B);
                    dataMonitor_2->setCurrent(current_B);
                    dataMonitor_2->setPower(power_B);
                }
            }
            break;
            
        case UI_Mode_DataChart_1:
            // Update chart data for current channel
            if (dataMonitor_1 && dataChart_1) {
                // Update monitor data for channel 1
                float voltage_1 = sensorDataMessage.data[0].busVoltage_mV / 1000.0f;
                float current_1 = sensorDataMessage.data[0].current_mA;
                float power_1 = sensorDataMessage.data[0].power_mW;
                if (dataMonitor_1->getContainer() && lv_obj_is_valid(dataMonitor_1->getContainer())) {
                    dataMonitor_1->setVoltage(voltage_1);
                    dataMonitor_1->setCurrent(current_1);
                    dataMonitor_1->setPower(power_1);
                }
                // Update chart data for channel 1
                if (dataChart_1->getSeries() && lv_obj_is_valid(dataChart_1->getContainer())) {
                    dataChart_1->addDataPoint(current_1);
                }
            }
            else {
                Serial.println("ERROR: Data Monitor or Chart not found!");
            }
            break;
            case UI_Mode_DataChart_2:
            // Update chart data for current channel
            if (dataMonitor_2 && dataChart_2) {
                // Update monitor data for channel 1
                float voltage_2 = sensorDataMessage.data[1].busVoltage_mV / 1000.0f;
                float current_2 = sensorDataMessage.data[1].current_mA;
                float power_2 = sensorDataMessage.data[1].power_mW;
                if (dataMonitor_2->getContainer() && lv_obj_is_valid(dataMonitor_2->getContainer())) {
                    dataMonitor_2->setVoltage(voltage_2);
                    dataMonitor_2->setCurrent(current_2);
                    dataMonitor_2->setPower(power_2);
                }
                // Update chart data for channel 1
                if (dataChart_2->getSeries() && lv_obj_is_valid(dataChart_2->getContainer())) {
                    dataChart_2->addDataPoint(current_2);
                }
            }
            else {
                Serial.println("ERROR: Data Monitor or Chart not found!");
            }
            break;
            
        case UI_Mode_DataCount_1:
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
        current_UI_mode = mode;
    }
}

void UI_manager::switch_UI_next()
{
    //Switch order: DataMonitor -> DataChart_1 -> DataChart_2
    switch (current_UI_mode) {
        case UI_Mode_DataMonitor:
            switch_UI(UI_Mode_DataChart_1);
            break;
        case UI_Mode_DataChart_1:
            switch_UI(UI_Mode_DataChart_2);
            break;
        case UI_Mode_DataChart_2:
            switch_UI(UI_Mode_DataCount_1);
            break;
        case UI_Mode_DataCount_1:
            switch_UI(UI_Mode_DataMonitor);
            break;
    }
}

void UI_manager::switch_UI_prev()
{
    //Switch order: DataCount_1 -> DataChart_2 -> DataChart_1 -> DataMonitor
    switch (current_UI_mode) {
        case UI_Mode_DataCount_1:
            switch_UI(UI_Mode_DataChart_2);
            break;
        case UI_Mode_DataChart_2:
            switch_UI(UI_Mode_DataChart_1);
            break;
        case UI_Mode_DataChart_1:
            switch_UI(UI_Mode_DataMonitor);
            break;
        case UI_Mode_DataMonitor:
            switch_UI(UI_Mode_DataCount_1);
            break;

    }
}





