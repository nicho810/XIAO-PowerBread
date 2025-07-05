#include "lvgl_keyboard.h"

extern QueueHandle_t buttonEventQueue;
extern UI_manager ui_manager;

void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    ButtonEventMessage message;
    
    // Try to receive a message from the queue (non-blocking)
    if (xQueueReceive(buttonEventQueue, &message, 0) == pdTRUE)
    {
        uint8_t key_map = 0;

        //Debug print
        // Serial.printf("Button handler: %d\n", message.buttonState.key_shortPressed_value);
        // Serial.flush();

        // Map button values to LVGL keys
        switch (message.buttonState.key_shortPressed_value) {
            case 1: key_map = LV_KEY_UP; break;
            case 2: key_map = LV_KEY_ENTER; break;
            case 3: key_map = LV_KEY_DOWN; break;
            default: key_map = 0; break;
        }
        
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = key_map;
        
        // Debug: Print the key being sent to LVGL
        // Serial.printf("LVGL key sent: %d\n", key_map);
        // Serial.flush();
    } else {
        // No message in queue, return released state
        data->state = LV_INDEV_STATE_RELEASED;
        data->key = 0;
    }
}

// Key event callback function
void key_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // Serial.printf("Event received: %d\n", code); // Debug print
    // Serial.flush();
    
    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        // Serial.printf("Key event received: %d\n", key); // Debug print
        // Serial.flush();
        
        switch (key)
        {
        case LV_KEY_UP:
            Serial.println("UP key pressed");
            Serial.flush();
            ui_manager.switch_UI_prev();
            break;

        case LV_KEY_DOWN:
            Serial.println("DOWN key pressed");
            Serial.flush();
            ui_manager.switch_UI_next();
            break;

        case LV_KEY_ENTER:
            Serial.println("ENTER key pressed");
            Serial.flush();
            break;
        }
    }
}


// Add event handling to the container
void setup_container_events(lv_obj_t *container)
{
    // Make container focusable
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(container, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    // Add to default group
    lv_group_t *g = lv_group_get_default();
    if (g)
    {
        lv_group_add_obj(g, container);
        Serial.println("Container added to group"); // Debug print
        Serial.flush();
    } else {
        Serial.println("No default group found!"); // Debug print
        Serial.flush();
    }
    
    // Add key event handler
    lv_obj_add_event_cb(container, key_event_cb, LV_EVENT_KEY, NULL);
    Serial.println("Key event callback added to container"); // Debug print
    Serial.flush();
    
    // Focus the container
    lv_group_focus_obj(container);
    Serial.println("Container focused"); // Debug print
    Serial.flush();
}







