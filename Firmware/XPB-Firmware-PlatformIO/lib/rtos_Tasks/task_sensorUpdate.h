#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include <lvgl.h>
#if defined(Proj_XIAOPowerBread)
#include "CurrentSensor_INA3221.h"
#elif defined(Proj_XIAOPowerMonitor)
#include "CurrentSensor_INA226.h"
#endif
#include "lvgl_ui.h"
#include "lvgl_ui_updateFunc.h"
#include "function_mode.h"


// Function declarations
void sensorUpdateTask(void *pvParameters);

// External variable declarations
extern SemaphoreHandle_t lvglMutex;
extern SemaphoreHandle_t xSemaphore;
#if defined(Proj_XIAOPowerBread)
extern CurrentSensor_INA3221 c_Sensor;
#elif defined(Proj_XIAOPowerMonitor)
extern CurrentSensor_DualINA226 c_Sensor;
#endif
extern DualChannelData latestSensorData;
extern lv_obj_t *ui_container;
extern volatile function_mode current_functionMode;
extern volatile bool functionMode_ChangeRequested;
extern volatile bool highLightChannel_ChangeRequested;
extern uint8_t highLightChannel;
extern uint8_t forceUpdate_flag;

extern volatile int tft_Rotation;

// Averaging arrays
extern float avgS[2], avgM[2], avgH[2], peak[2];
