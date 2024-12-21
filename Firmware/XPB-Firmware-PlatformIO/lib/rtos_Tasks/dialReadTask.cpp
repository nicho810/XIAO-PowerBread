#include "dialReadTask.h"

// Keyboard state variables (remove static keyword for shared variables)
bool last_key_pressed = false;
uint32_t last_key = 0;

// These can remain static as they're only used within this file
static uint32_t enter_press_start = 0;
static const uint32_t LONG_PRESS_DURATION = 500; // 500ms for long press
static bool long_press_triggered = false;

void update_keyboard_state(uint8_t dialStatus)
{
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
                return;
        }
        last_key_pressed = true;
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

    // Add debounce variables
    static uint8_t lastStableStatus = 0;
    static TickType_t lastChangeTime = 0;
    const TickType_t debounceDelay = pdMS_TO_TICKS(20); // 20ms debounce delay

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
                    // Serial.printf("Dial turn: %d\n", dialStatus);
                    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
                        update_keyboard_state(dialStatus);
                        xSemaphoreGive(lvglMutex);
                    }
                    vTaskDelay(pdMS_TO_TICKS(300)); // Debounce delay
                    break;

                case 3:  // Pressed
                    if (pressStartTime == 0) {
                        pressStartTime = xCurrentTime;
                        shortPressHandled = false;
                        longPressDetected = false;
                    }
                    else if ((xCurrentTime - pressStartTime) >= longPressThreshold && !longPressDetected) {
                        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
                            update_keyboard_state(4); // 4 = Long press Dial
                            xSemaphoreGive(lvglMutex);
                        }
                        // Serial.printf("Dial long pressed: %d\n", dialStatus);
                        longPressDetected = true;  // Set the flag when long press is detected
                        vTaskDelay(pdMS_TO_TICKS(100)); // Shorter debounce for better responsiveness
                    }
                    break;

                case 0:  // Reset/Released
                    if (pressStartTime != 0) {  // Only process if there was a press
                        if (!longPressDetected && !shortPressHandled) {
                            // This was a short press
                            // Serial.printf("Dial short pressed: %d\n", dialStatus);
                            if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
                                update_keyboard_state(3); // 3 = Short press Dial
                                xSemaphoreGive(lvglMutex); 
                            }
                            shortPressHandled = true;
                        }
                        // Reset all press tracking variables
                        pressStartTime = 0;
                        longPressDetected = false;
                        vTaskDelay(pdMS_TO_TICKS(100)); // Debounce on release
                    }
                    break;
            }
            lastDialStatus = dialStatus;
            xLastReadTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}