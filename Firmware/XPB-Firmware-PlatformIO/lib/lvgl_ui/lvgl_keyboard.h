#ifndef LVGL_KEYBOARD_H
#define LVGL_KEYBOARD_H

#include "lvgl.h"
#include "input_ButtonX3.h"
#include "lvgl_uiManager.h"

void setup_container_events(lv_obj_t *container); // Add event handling to the container
void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data); // LVGL Input Device Keyboard read function
void key_event_cb(lv_event_t *e); // LVGL key event callback function


#endif