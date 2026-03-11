/**
 * [INPUT]: Arduino.h, boardConfig.h, lvgl.h, INA3221Sensor.h, lvgl_ui.h, lvgl_ui_updateFunc.h, function_mode.h
 * [OUTPUT]: sensorUpdateTask() 任务函数, latestSensorData (extern), avgS/avgM/avgH/peak (extern)
 * [POS]: 传感器采样+均值计算+选择性UI刷新 (优先级3)，系统数据流的枢纽
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include <lvgl.h>
#include "INA3221Sensor.h"
#include "lvgl_ui.h"
#include "lvgl_ui_updateFunc.h"
#include "function_mode.h"


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
extern volatile bool highLightChannel_ChangeRequested;
extern uint8_t highLightChannel;
extern uint8_t forceUpdate_flag;

extern volatile int tft_Rotation;

// Averaging arrays
extern float avgS[2], avgM[2], avgH[2], peak[2];
