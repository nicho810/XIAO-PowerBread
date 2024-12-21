#ifndef DIALREADTASK_H
#define DIALREADTASK_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "dialSwitch.h"
#include "lvgl.h"

// External declarations
extern SemaphoreHandle_t lvglMutex;
extern DialFunction dial;
extern volatile int dialStatus;
extern volatile int lastDialStatus;

// Make keyboard state variables accessible
extern bool last_key_pressed;
extern uint32_t last_key;

// Function declaration
void dialReadTask(void *pvParameters);
void update_keyboard_state(uint8_t dialStatus);

#endif