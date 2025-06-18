#include "lvgl_func.h"


// extern SemaphoreHandle_t xSemaphore;
// extern volatile bool last_key_pressed;
// extern volatile uint32_t last_key;

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

      // Initialize basic screen
    lv_obj_t * scr = lv_scr_act();
    if (!scr) {
        Serial.println("> ERROR: Failed to get active screen!");
        while(1) delay(100);
    }

    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    // Create a test label
    lv_obj_t * label = lv_label_create(scr);
    if (!label) {
        Serial.println("> ERROR: Failed to create test label!");
        while(1) delay(100);
    }

    lv_label_set_text(label, "Booting...");
    lv_obj_center(label);

    // Force a refresh
    lv_refr_now(disp);

}

// void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
// {
//     // Take semaphore with a short timeout
//     if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
//     {
//         if (last_key_pressed)
//         {
//             data->state = LV_INDEV_STATE_PRESSED;
//             data->key = last_key;
//         }
//         else
//         {
//             data->state = LV_INDEV_STATE_RELEASED;
//             data->key = last_key;
//         }

//         // Reset the key press state after it's been read
//         last_key_pressed = false;

//         xSemaphoreGive(xSemaphore);
//     }
//     else
//     {
//         // If we couldn't get the semaphore, maintain previous state
//         data->state = LV_INDEV_STATE_RELEASED;
//         data->key = 0;
//     }
// }