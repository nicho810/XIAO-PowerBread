#include "lvgl_func.h"
#include "input_ButtonX3.h"
#include "task_input.h"

extern SemaphoreHandle_t lvglMutex;
extern ButtonState_X3 buttonState_X3;
extern InputButtonX3 input_buttonX3;
extern lv_indev_drv_t indev_drv;
extern lv_obj_t *ui_container;
extern QueueHandle_t buttonEventQueue;


void lvgl_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t len = w * h;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    // Replace individual pushColor with bulk writePixels
    tft.writePixels((uint16_t *)&color_p->full, len, true);

    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void lvgl_init(void)
{
    // LCD Init
    if (!tft.begin()) {  // Add error checking for LCD initialization
        Serial.println("> LCD initialization failed!");
        while(1) delay(100);
    }
    tft.setColorDepth(16);         // RGB565
    tft.setRotation(0); // Set initial hardware rotation = 0
    tft.fillScreen(0x0000);        // Black screen

    // Initialize LVGL
    lv_init();
    Serial.println("> LVGL core initialized");

     // Initialize display buffer with smaller size
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[screen_width * 10];
    static lv_color_t buf2[screen_width * 10];
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_width * 10);
    Serial.println("> Display buffer initialized");
    Serial.flush();

    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screen_width;
    disp_drv.ver_res = screen_height;
    disp_drv.flush_cb = lvgl_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    disp_drv.direct_mode = 0;
    disp_drv.antialiasing = 0;

    // Register the driver
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    if (!disp) {
        Serial.println("ERROR: Failed to register display driver!");
        while(1) delay(100);
    }
    Serial.println("Display driver registered");
    Serial.flush();

    //register keyboard input device
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv);

    // Set it as default input device
    if (kb_indev){
        lv_group_t *g = lv_group_create();
        lv_group_set_default(g);
        lv_indev_set_group(kb_indev, g);
        Serial.println("> Input device registered successfully");
    } else {
        Serial.println("> Failed to register input device!");
    }

    // Initialize basic screen
    lv_obj_t * scr = lv_scr_act();
    if (!scr) {
        Serial.println("> ERROR: Failed to get active screen!");
        while(1) delay(100);
    }

    lv_obj_clean(lv_scr_act());
    ui_container = lv_obj_create(lv_scr_act());
    if (!ui_container || !lv_obj_is_valid(ui_container)) {
        Serial.println("ERROR: Failed to create ui container!");
        while(1) delay(100);
    }

    // Add event handling to the container
    setup_container_events(ui_container);

    lv_obj_set_size(ui_container, screen_width, screen_height);
    lv_obj_set_style_radius(ui_container, 0, LV_PART_MAIN);
    lv_obj_center(ui_container);
    lv_obj_clear_flag(ui_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_container, 0, LV_PART_MAIN);

    // Force a refresh to ensure container is created
    lv_refr_now(lv_disp_get_default());
    Serial.println("UI container created successfully");
    Serial.flush();


    // Create a test label
    lv_obj_t * label = lv_label_create(ui_container);
    if (!label) {
        Serial.println("> ERROR: Failed to create test label!");
        while(1) delay(100);
    }

    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(label, "Booting...");
    lv_obj_center(label);

    // Force a refresh
    lv_refr_now(disp);
}

void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    ButtonEventMessage message;
    
    // Try to receive a message from the queue (non-blocking)
    if (xQueueReceive(buttonEventQueue, &message, 0) == pdTRUE)
    {
        uint8_t key_map = 0;

        // Map button values to LVGL keys
        switch (message.buttonState.key_shortPressed_value) {
            case 1: key_map = LV_KEY_UP; break;
            case 2: key_map = LV_KEY_ENTER; break;
            case 3: key_map = LV_KEY_DOWN; break;
            default: key_map = 0; break;
        }
        
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = key_map;
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
    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        switch (key)
        {
        case LV_KEY_UP:
            Serial.println("UP key pressed");
            Serial.flush();
            break;

        case LV_KEY_DOWN:
            Serial.println("DOWN key pressed");
            Serial.flush();
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
    }
    // Add key event handler
    lv_obj_add_event_cb(container, key_event_cb, LV_EVENT_KEY, NULL);
}