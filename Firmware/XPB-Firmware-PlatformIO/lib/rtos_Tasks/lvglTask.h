#ifndef LVGL_TASK_H
#define LVGL_TASK_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <lvgl.h>

// Function declaration
void lvglTask(void *parameter);

extern SemaphoreHandle_t lvglMutex;

#endif // LVGL_TASK_H
