/**
 * [INPUT]: Arduino.h, boardConfig.h, lvgl.h
 * [OUTPUT]: lvglTask() 任务函数, lvglMutex (extern)
 * [POS]: LVGL 渲染引擎 (优先级4)，5ms 周期调用 lv_timer_handler + lv_refr_now
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

#endif // LVGL_TASK_H
