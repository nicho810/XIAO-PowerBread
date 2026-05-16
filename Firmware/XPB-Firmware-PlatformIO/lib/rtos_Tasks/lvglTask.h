/**
 * [INPUT]: Arduino.h, boardConfig.h, lvgl.h, sensorUpdateTask.h (EVT_*), lvgl_ui_updateFunc.h
 * [OUTPUT]: lvglTask() 任务函数, lvglMutex (extern)
 * [POS]: UI 唯一控制者 (优先级4)，事件驱动渲染 + widget 更新 + lv_timer_handler
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef LVGL_TASK_H
#define LVGL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include <lvgl.h>

// Function declaration
void lvglTask(void *parameter);

extern SemaphoreHandle_t lvglMutex;
extern TaskHandle_t xLvglTaskHandle;

#endif // LVGL_TASK_H
