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
const TickType_t debounceDelay = pdMS_TO_TICKS(20); // 20ms debounce delay

void update_keyboard_state(uint8_t dialStatus)
{
    if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE) {  // Use xSemaphore instead of lvglMutex
        if (dialStatus != 0) { // If there's any dial activity
            switch (dialStatus) {
                case 1: // Up rotation
                    last_key = LV_KEY_UP;
                    break;
                case 2: // Down rotation
                    last_key = LV_KEY_DOWN;
                    break;
                case 3: // Press
                    last_key = LV_KEY_ENTER;
                    break;
                case 4: // Long press
                    last_key = LV_KEY_BACKSPACE;
                    break;
                default:
                    xSemaphoreGive(xSemaphore);
                    return;
            }
            last_key_pressed = true;
            Serial.printf("Dial status: %d, Key set: %d\n", dialStatus, last_key);  // Debug print
        }
        xSemaphoreGive(xSemaphore);
    }
}

// Dial Read Task
void dialReadTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastReadTime = 0;
    const TickType_t xReadInterval = pdMS_TO_TICKS(50); // Read every 50ms
    TickType_t pressStartTime = 0;
    const TickType_t longPressThreshold = pdMS_TO_TICKS(700); // 1 second
    bool longPressDetected = false;
    bool shortPressHandled = false;

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastReadTime) >= xReadInterval)
        {
            uint8_t rawStatus = dial.readDialStatus();// Read the raw dial status
            // Apply debounce
            if (rawStatus != lastStableStatus){
                if ((xCurrentTime - lastChangeTime) >= debounceDelay) // Status has been stable for the debounce period
                {
                    dialStatus = rawStatus;
                    lastStableStatus = rawStatus;
                }
                lastChangeTime = xCurrentTime;
            }

            // Handle rotation changes when dial is turned
            switch (dialStatus) {
                case 1:  // Up rotation
                case 2:  // Down rotation
                    update_keyboard_state(dialStatus);
                    vTaskDelay(pdMS_TO_TICKS(10));  // Reduced to 10ms
                    break;

                case 3:  // Pressed
                    if (pressStartTime == 0) {
                        pressStartTime = xCurrentTime;
                        shortPressHandled = false;
                        longPressDetected = false;
                    }
                    else if ((xCurrentTime - pressStartTime) >= longPressThreshold && !longPressDetected) {
                        update_keyboard_state(4); // 4 = Long press Dial
                        longPressDetected = true;
                        vTaskDelay(pdMS_TO_TICKS(5));  // Reduced to 5ms
                    }
                    break;

                case 0:  // Reset/Released
                    if (pressStartTime != 0) {
                        if (!longPressDetected && !shortPressHandled) {
                            update_keyboard_state(3); // 3 = Short press Dial
                            shortPressHandled = true;
                        }
                        pressStartTime = 0;
                        longPressDetected = false;
                        vTaskDelay(pdMS_TO_TICKS(5));  // Reduced to 5ms
                    }
                    break;
            }
            lastDialStatus = dialStatus;
            xLastReadTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}