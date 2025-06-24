#ifndef TASK_INPUT_H
#define TASK_INPUT_H

#include <Arduino.h>
#include "boardConfig.h"
#include "input_ButtonX3.h"


// Function declarations
void inputTask(void *pvParameters);

// External variable declarations
extern QueueHandle_t buttonEventQueue;
extern InputButtonX3 input_buttonX3;
extern ButtonState_X3 buttonState_X3;

extern InputButtonX3 input_buttonX3;
extern ButtonState_X3 buttonState_X3;



#endif