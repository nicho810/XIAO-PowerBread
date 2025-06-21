#include "task_sensor.h"

void sensorTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xInterval = pdMS_TO_TICKS(100); // 100ms update interval

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastWakeTime) >= xInterval)
        {
            // Read sensor data
            std::vector<currentSensorData> cSensorData_tmp;
            currentSensor_2ch.readData(cSensorData_tmp);
            
            // Prepare message for queue
            SensorDataMessage message;
            for (int i = 0; i < sizeof(message.data)/sizeof(message.data[0]); i++) {
                message.data[i] = cSensorData_tmp[i];
            }
            message.timestamp = xCurrentTime;
            
            // Send to queue (non-blocking with timeout)
            if (xQueueOverwrite(sensorDataQueue, &message) != pdTRUE) {
                // Queue is full, handle accordingly
                Serial.println("Warning: Sensor data queue full - dropping data");
            }
            
            xLastWakeTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}



/*
// Sensor Update Task
void sensorUpdateTask(void *pvParameters)
{
    (void)pvParameters;
    Serial.println("> Sensor task starting...");
    Serial.flush();

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(5);
    // const float UPDATE_THRESHOLD = 0.005f; // 5mV/mA threshold

    // // Static buffers for averaging
    // static const int SHORT_SAMPLES = 100; // 1 second
    // static const int MED_SAMPLES = 1000;  // 10 seconds
    // static const int LONG_SAMPLES = 6000; // 60 seconds
    // static float shortBuffer[2][SHORT_SAMPLES] = {{0}};
    // static float medBuffer[2][MED_SAMPLES] = {{0}};
    // static float longBuffer[2][LONG_SAMPLES] = {{0}};
    // static int shortIndex = 0;
    // static int medIndex = 0;
    // static int longIndex = 0;

    // // Add these static variables at the start of sensorUpdateTask
    // static bool buffersInitialized = false;
    // static int sampleCount[2] = {0, 0};
    // static bool first_ui_update = true;

    // Add task initialization debug
    Serial.println("> Sensor task initialized");
    Serial.flush();

    while (1)
    {
            Serial.println("> Sensor task cycle");  // Add this debug line
            Serial.flush();
            
            vTaskDelayUntil(&xLastWakeTime, xFrequency);

            // Take the main semaphore first
            if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
            {
                bool isInConfigMode = false;
                bool needsConfigExit = false;
                
                // Check config mode with proper timeout
                if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                    isInConfigMode = configMode.configState.isActive;
                    xSemaphoreGive(configStateMutex);
                }
                
                if (isInConfigMode)
                {
                    // Handle config mode updates with proper mutex handling
                    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(10)) == pdTRUE)
                    {
                        lv_obj_t *item_area = lv_obj_get_child(ui_container, 1);
                        if (configMode.configState.cursorStatus >= 0)
                        {
                            // Skip update if cursor hasn't changed
                            static int8_t last_cursor = -1;
                            static int8_t last_status = -1;

                            if (last_cursor != configMode.configState.cursor ||
                                last_status != configMode.configState.cursorStatus)
                            {
                                update_configMode(item_area,
                                                  configMode.configState.cursor,
                                                  configMode.configState.cursorLast,
                                                  configMode.configState.cursorMax,
                                                  configMode.configState.cursorStatus);

                                last_cursor = configMode.configState.cursor;
                                last_status = configMode.configState.cursorStatus;
                            }

                            // update the value of the item if cursorStatus is 1(selected)
                            if (configMode.configState.cursorStatus == 1)
                            {
                                update_configMode_cfgData(item_area, configMode.configState.cursor);
                            }
                        }
                        else if (configMode.configState.cursorStatus == -1)
                        {
                            needsConfigExit = true;
                        }
                        xSemaphoreGive(lvglMutex);
                    }
                    
                    // Handle config mode exit outside of LVGL mutex
                    if (needsConfigExit) {
                        configModeExitRequested = true;
                        Serial.println("> Config mode exit requested by sensor task");
                        Serial.flush();
                    }
                }
                else // Only process sensor updates when not in config mode
                {
                    DualChannelData newSensorData = c_Sensor.readCurrentSensors();

                    // if the function mode is changed, reinit the ui
                    if (functionMode_ChangeRequested)
                    {
                        // Skip the first UI change request since we just initialized in setup()
                        if (first_ui_update) {
                            first_ui_update = false;
                            functionMode_ChangeRequested = false;
                            continue;
                        }

                        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
                        {
                            // Delete old container first
                            if (ui_container != NULL)
                            {
                                lv_obj_del(ui_container);
                                ui_container = NULL;
                            }

                            // Create new base container
                            ui_container = lv_obj_create(lv_scr_act());
                            if (ui_container) {
                                lv_obj_set_size(ui_container, screen_width, screen_height);
                                lv_obj_center(ui_container);
                                lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
                                lv_obj_set_style_bg_color(ui_container, lv_color_black(), LV_PART_MAIN);
                                lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN);

                                // Initialize UI with the new container
                                initUI(current_functionMode, ui_container, highLightChannel, latestSensorData, 0.0f, 0);
                            }

                            // Reset latest sensor data to force next update
                            latestSensorData.channel0.busCurrent = -999.0f;
                            latestSensorData.channel1.busCurrent = -999.0f;
                            latestSensorData.channel0.busVoltage = -999.0f;
                            latestSensorData.channel1.busVoltage = -999.0f;

                            forceUpdate_flag = true;
                            xSemaphoreGive(lvglMutex);
                        }
                    }

                    // Calculate averages for dataMonitorCount mode
                    if (current_functionMode == Mode_3)
                    {
                        for (int ch = 0; ch < 2; ch++)
                        {
                            float currentVal = (ch == 0) ? newSensorData.channel0.busCurrent
                                                         : newSensorData.channel1.busCurrent;

                            // 1-second moving average
                            float alphaS = 0.3f; // Faster response time
                            avgS[ch] = (alphaS * currentVal) + ((1.0f - alphaS) * avgS[ch]);

                            // 1-minute moving average
                            float alphaM = 0.002f;
                            avgM[ch] = (alphaM * currentVal) + ((1.0f - alphaM) * avgM[ch]);

                            // 1-hour moving average
                            float alphaH = 0.000033f;
                            avgH[ch] = (alphaH * currentVal) + ((1.0f - alphaH) * avgH[ch]);

                            // Update peak values
                            if (currentVal > peak[ch])
                            {
                                peak[ch] = currentVal;
                            }
                        }

                        // Update buffer indices
                        shortIndex = (shortIndex + 1) % SHORT_SAMPLES;
                        medIndex = (medIndex + 1) % MED_SAMPLES;
                        longIndex = (longIndex + 1) % LONG_SAMPLES;
                    }

                    // Check if display update is needed
                    bool shouldUpdate = forceUpdate_flag;
                    if (!shouldUpdate)
                    {
                        // Force update if latestSensorData contains invalid values
                        if (latestSensorData.channel0.busCurrent < -900.0f ||
                            latestSensorData.channel1.busCurrent < -900.0f)
                        {
                            shouldUpdate = true;
                        }
                        // Always update for Chart and Count modes
                        else if (current_functionMode == Mode_2 ||
                                 current_functionMode == Mode_3)
                        {
                            shouldUpdate = true;
                        }
                        else
                        {
                            // For other modes, only update on significant changes
                            shouldUpdate = (abs(newSensorData.channel0.busCurrent - latestSensorData.channel0.busCurrent) > UPDATE_THRESHOLD) ||
                                           (abs(newSensorData.channel1.busCurrent - latestSensorData.channel1.busCurrent) > UPDATE_THRESHOLD) ||
                                           (abs(newSensorData.channel0.busVoltage - latestSensorData.channel0.busVoltage) > UPDATE_THRESHOLD) ||
                                           (abs(newSensorData.channel1.busVoltage - latestSensorData.channel1.busVoltage) > UPDATE_THRESHOLD);
                        }
                    }

                    if (shouldUpdate || highLightChannel_ChangeRequested)
                    {
                        latestSensorData = newSensorData;

                        if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(10)) == pdTRUE)
                        {
                            lv_obj_t *container0 = lv_obj_get_child(ui_container, 0);

                            if (container0)
                            {
                                switch (current_functionMode)
                                {
                                case Mode_1: //basic monitor
                                    if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                                    {
                                        update_monitor_data(container0, 0, latestSensorData, 0.0f, 0);
                                        update_monitor_data(container1, 1, latestSensorData, 0.0f, 0);
                                    }
                                    break;

                                case Mode_2: //chart
                                    if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                                    {
                                        update_monitor_data(container0, highLightChannel, latestSensorData, 0.0f, 0);
                                        float currentValue = (highLightChannel == 0) ? latestSensorData.channel0.busCurrent : latestSensorData.channel1.busCurrent;
                                        update_chart_data(container1, highLightChannel, latestSensorData, 0, currentValue);
                                    }
                                    break;

                                case Mode_3: //count
                                    if (highLightChannel_ChangeRequested)
                                    {
                                        avgS[highLightChannel] = 0;
                                        avgM[highLightChannel] = 0;
                                        avgH[highLightChannel] = 0;
                                        peak[highLightChannel] = 0;
                                    }
                                    update_monitor_data(container0, highLightChannel, latestSensorData, 0.0f, 0);
                                    lv_obj_t *container1 = lv_obj_get_child(ui_container, 1);
                                    update_count_data(container1, highLightChannel, latestSensorData, avgS[highLightChannel], 0);
                                    container1 = lv_obj_get_child(ui_container, 2);
                                    update_count_data(container1, highLightChannel, latestSensorData, avgM[highLightChannel], 0);
                                    container1 = lv_obj_get_child(ui_container, 3);
                                    update_count_data(container1, highLightChannel, latestSensorData, avgH[highLightChannel], 0);
                                    container1 = lv_obj_get_child(ui_container, 4);
                                    update_count_data(container1, highLightChannel, latestSensorData, peak[highLightChannel], 0);
                                    break;
                                }
                            }
                            xSemaphoreGive(lvglMutex);
                        }

                        forceUpdate_flag = false;
                        highLightChannel_ChangeRequested = false;
                    }
                }
                
                xSemaphoreGive(xSemaphore);
            }
    
        
        // Always include a small delay to prevent tight loops
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

*/