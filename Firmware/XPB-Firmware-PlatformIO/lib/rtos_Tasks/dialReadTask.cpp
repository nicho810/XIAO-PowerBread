#include "dialReadTask.h"
extern SemaphoreHandle_t xSemaphore;

// Keyboard state variables (remove static keyword for shared variables)
bool last_key_pressed = false;
uint32_t last_key = 0;

// These can remain static as they're only used within this file
static uint32_t enter_press_start = 0;
static const uint32_t LONG_PRESS_DURATION = 500; // 500ms for long press
static bool long_press_triggered = false;

// Add debounce variables
static uint8_t lastStableStatus = 0;
static TickType_t lastChangeTime = 0;
const TickType_t debounceDelay = pdMS_TO_TICKS(100); // 100ms debounce delay

void update_keyboard_state(uint8_t status)
{
    if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        last_key_pressed = true;
        switch (status)
        {
        case 1: // Up
            last_key = LV_KEY_UP;
            break;
        case 2: // Down
            last_key = LV_KEY_DOWN;
            break;
        case 3: // Short press
            last_key = LV_KEY_ENTER;
            break;
        case 4: // Long press
            last_key = LV_KEY_ESC;
            break;
        default:
            last_key_pressed = false;
            last_key = 0;
            break;
        }
        xSemaphoreGive(xSemaphore);
    }
}

// Dial Read Task
void dialReadTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastReadTime = 0;
    const TickType_t xReadInterval = pdMS_TO_TICKS(100);
    TickType_t pressStartTime = 0;
    const TickType_t longPressThreshold = pdMS_TO_TICKS(700);
    bool longPressDetected = false;
    bool shortPressHandled = false;
    bool isPressed = false;

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastReadTime) >= xReadInterval)
        {
            uint8_t rawStatus = dial.readDialStatus();
            // Apply debounce
            if (rawStatus != lastStableStatus) {
                if ((xCurrentTime - lastChangeTime) >= debounceDelay)
                {
                    dialStatus = rawStatus;
                    lastStableStatus = rawStatus;
                    
                    // Handle press start
                    if (dialStatus == 3) {
                        if (!isPressed) {
                            isPressed = true;
                            pressStartTime = xCurrentTime;
                            longPressDetected = false;
                            shortPressHandled = false;
                        }
                    }
                    // Handle release
                    else if (dialStatus == 0 && isPressed) {
                        isPressed = false;
                        // If it wasn't a long press, then it's a short press
                        if (!longPressDetected) {
                            update_keyboard_state(3); // Short press
                            shortPressHandled = true;
                            vTaskDelay(pdMS_TO_TICKS(200)); // Add delay after short press
                        }
                        // Reset press states
                        pressStartTime = 0;
                        longPressDetected = false;
                        shortPressHandled = false;
                    }
                }
                lastChangeTime = xCurrentTime;
            }

            // Handle different states
            switch (dialStatus) {
                case 1:  // Up rotation
                case 2:  // Down rotation
                    update_keyboard_state(dialStatus);
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;

                case 3:  // Pressed
                    if (isPressed && !longPressDetected && 
                        (xCurrentTime - pressStartTime) >= longPressThreshold) {
                        update_keyboard_state(4); // Long press
                        longPressDetected = true;
                        vTaskDelay(pdMS_TO_TICKS(200));
                    }
                    break;

                case 0:  // Released
                    // Reset key state after a delay
                    if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE) {
                        last_key_pressed = false;
                        last_key = 0;
                        xSemaphoreGive(xSemaphore);
                    }
                    break;
            }
            
            lastDialStatus = dialStatus;
            xLastReadTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}