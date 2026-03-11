/**
 * [INPUT]: Arduino.h, boardConfig.h, INA3221Sensor.h, function_mode.h
 * [OUTPUT]: sensorUpdateTask() 任务函数, EVT_* 事件位, latestSensorData/avg/peak (extern)
 * [POS]: 纯数据生产者 (优先级3)，5ms 周期 I2C 采集 + EMA 计算，零 LVGL 依赖
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include "INA3221Sensor.h"
#include "function_mode.h"

/* ================================================================
 *  TaskNotify 事件位 — key_event_cb → sensorUpdateTask → lvglTask
 * ================================================================ */
#define EVT_MODE_CHANGE        (1 << 0)
#define EVT_HIGHLIGHT_CHANGE   (1 << 1)
#define EVT_FORCE_UPDATE       (1 << 2)

// Function declarations
void sensorUpdateTask(void *pvParameters);

// External variable declarations
extern SemaphoreHandle_t sensorDataMutex;
extern INA3221Sensor inaSensor;
extern DualChannelData latestSensorData;
extern volatile function_mode current_functionMode;
extern uint8_t highLightChannel;
extern TaskHandle_t xSensorTaskHandle;

// Averaging arrays
extern float avgS[2], avgM[2], avgH[2], peak[2];
