#include "sensorUpdateTask.h"

// Sensor Update Task
void sensorUpdateTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10);
    const float UPDATE_THRESHOLD = 0.005f; // 5mV/mA threshold

    // Static buffers for averaging
    static const int SHORT_SAMPLES = 100; // 1 second
    static const int MED_SAMPLES = 1000;  // 10 seconds
    static const int LONG_SAMPLES = 6000; // 60 seconds
    static float shortBuffer[2][SHORT_SAMPLES] = {{0}};
    static float medBuffer[2][MED_SAMPLES] = {{0}};
    static float longBuffer[2][LONG_SAMPLES] = {{0}};
    static int shortIndex = 0;
    static int medIndex = 0;
    static int longIndex = 0;

    // Add these static variables at the start of sensorUpdateTask
    static bool buffersInitialized = false;
    static int sampleCount[2] = {0, 0};

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            DualChannelData newSensorData = inaSensor.readCurrentSensors();

            //if the function mode is changed, reinit the ui
            if (functionMode_ChangeRequested)
            {
                // Take LVGL mutex before UI operations
                if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
                {
                    // Delete old container first
                    if (ui_container != NULL) {
                        lv_obj_del(ui_container);
                        ui_container = NULL;
                    }

                    // Create new UI
                    switch (current_functionMode)
                    {
                    case dataMonitor:
                        ui_container = dataMonitor_initUI(tft_Rotation);
                        break;
                    case dataMonitorChart:
                        ui_container = dataMonitorChart_initUI(tft_Rotation, highLightChannel);
                        break;
                    case dataMonitorCount:
                        ui_container = dataMonitorCount_initUI(tft_Rotation, highLightChannel);
                        break;
                    default:
                        ui_container = dataMonitor_initUI(tft_Rotation);
                        break;
                    }

                    forceUpdate_flag = true; //force update the ui after mode change

                    // Ensure UI was created successfully
                    if (ui_container == NULL) {
                        // Fallback to default UI if creation failed
                        ui_container = dataMonitor_initUI(tft_Rotation);
                    }

                    xSemaphoreGive(lvglMutex);
                }
                functionMode_ChangeRequested = false;
            }

            // Calculate averages for dataMonitorCount mode
            if (current_functionMode == dataMonitorCount)
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
            if (!shouldUpdate) {
                // Always update for Chart and Count modes
                if (current_functionMode == dataMonitorChart || 
                    current_functionMode == dataMonitorCount) {
                    shouldUpdate = true;
                } else {
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

                if (xSemaphoreTake(lvglMutex, 0) == pdTRUE)
                {
                    lv_obj_t *container0 = lv_obj_get_child(ui_container, 0);

                    if (container0)
                    {
                        switch (current_functionMode)
                        {
                        case dataMonitor:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, 0, latestSensorData);
                                update_monitor_data(container1, 1, latestSensorData);
                            }
                            break;

                        case dataMonitorChart:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, highLightChannel, latestSensorData);
                                float currentValue = (highLightChannel == 0) ? 
                                    latestSensorData.channel0.busCurrent : 
                                    latestSensorData.channel1.busCurrent;
                                update_chart_data(container1, currentValue);
                            }
                            break;

                        case dataMonitorCount:
                            if (highLightChannel_ChangeRequested)
                            {
                                avgS[highLightChannel] = 0;
                                avgM[highLightChannel] = 0;
                                avgH[highLightChannel] = 0;
                                peak[highLightChannel] = 0;
                            }
                            update_monitor_data(container0, highLightChannel, latestSensorData);
                            lv_obj_t *container1 = lv_obj_get_child(ui_container, 1);
                            update_count_data(container1, highLightChannel, avgS[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 2);
                            update_count_data(container1, highLightChannel, avgM[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 3);
                            update_count_data(container1, highLightChannel, avgH[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 4);
                            update_count_data(container1, highLightChannel, peak[highLightChannel]);
                            break;
                        }
                    }
                    xSemaphoreGive(lvglMutex);
                }
                forceUpdate_flag = false;
                highLightChannel_ChangeRequested = false;
            }

            xSemaphoreGive(xSemaphore);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}