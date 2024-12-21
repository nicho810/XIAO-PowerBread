#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <lvgl.h>
#include "INA3221Sensor.h"
#include "lvgl_ui_updateFunc.h"

// Function mode enum definition (moved from main file)
enum function_mode
{
    dataMonitor,
    dataMonitorChart,
    dataMonitorCount,
    // serialMonitor,
    // pwmOutput,
    // analogInputMonitor,
};

// Function declarations
void sensorUpdateTask(void *pvParameters);

// External variable declarations
extern SemaphoreHandle_t lvglMutex;
extern SemaphoreHandle_t xSemaphore;
extern INA3221Sensor inaSensor;
extern DualChannelData latestSensorData;
extern lv_obj_t *ui_container;
extern volatile function_mode current_functionMode;
extern volatile bool functionMode_ChangeRequested;
extern uint8_t highLightChannel;
extern uint8_t forceUpdate_flag;

// Averaging arrays
extern float avgS[2], avgM[2], avgH[2], peak[2];
