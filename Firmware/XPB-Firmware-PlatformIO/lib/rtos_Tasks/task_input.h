#ifndef TASK_INPUT_H
#define TASK_INPUT_H

#include <Arduino.h>
#include "boardConfig.h"
#include "input_ButtonX3.h"

// Queue message structure
struct ButtonEventMessage {
    ButtonState_X3 buttonState;
    uint32_t timestamp;
};

// Function declarations
void inputTask(void *pvParameters);

// External variable declarations
extern QueueHandle_t buttonEventQueue;
extern InputButtonX3 input_buttonX3;
extern ButtonState_X3 buttonState_X3;

extern InputButtonX3 input_buttonX3;
extern ButtonState_X3 buttonState_X3;



#endif