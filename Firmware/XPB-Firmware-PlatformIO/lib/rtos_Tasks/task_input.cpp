#include "task_input.h"

extern SemaphoreHandle_t xSemaphore;
extern InputButtonX3 input_buttonX3;
extern ButtonState_X3 buttonState_X3;


// Dial Read Task
void inputTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = 0;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // Read frequency

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastWakeTime) >= xFrequency)
        {
            // Serial.print("> Input task running:"); // Debug print
            // Serial.println(input_buttonX3.update(buttonState_X3));
            // Serial.flush();
            input_buttonX3.update(buttonState_X3);
            xLastWakeTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}





// // Keyboard state variables (remove static keyword for shared variables)
// bool last_key_pressed = false;
// uint32_t last_key = 0;

// // These can remain static as they're only used within this file
// static uint32_t enter_press_start = 0;
// static const uint32_t LONG_PRESS_DURATION = 500; // 500ms for long press
// static bool long_press_triggered = false;

// // Add debounce variables
// static uint8_t lastStableStatus = 0;
// static TickType_t lastChangeTime = 0;
// const TickType_t debounceDelay = pdMS_TO_TICKS(100); // 100ms debounce delay

// void update_keyboard_state(uint8_t status)
// {
//     if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
//     {
//         last_key_pressed = true;
//         switch (status)
//         {
//         case 1: // Up
//             last_key = LV_KEY_UP;
//             break;
//         case 2: // Down
//             last_key = LV_KEY_DOWN;
//             break;
//         case 3: // Short press
//             last_key = LV_KEY_ENTER;
//             break;
//         case 4: // Long press
//             last_key = LV_KEY_ESC;
//             break;
//         default:
//             last_key_pressed = false;
//             last_key = 0;
//             break;
//         }
//         xSemaphoreGive(xSemaphore);
//     }
// }

