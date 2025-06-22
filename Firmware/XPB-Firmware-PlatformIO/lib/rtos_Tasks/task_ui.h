#ifndef TASK_UI_H
#define TASK_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl_ui.h"

extern UI_manager ui_manager;
extern QueueHandle_t sensorDataQueue;
extern QueueHandle_t buttonEventQueue;
extern SemaphoreHandle_t lvglMutex;


void uiTask(void* pvParameters);

#endif