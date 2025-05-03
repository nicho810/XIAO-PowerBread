#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "xpb_color_palette.h"
#include "lvgl_ui_widget.h"
#include "function_mode.h"
#include "configMode.h"

// Add this global variable declaration
extern volatile bool menu_is_visible;

// External declarations for global variables
extern volatile bool functionMode_ChangeRequested;
extern volatile bool highLightChannel_ChangeRequested;
extern volatile function_mode current_functionMode;
extern SemaphoreHandle_t xSemaphore;  // Declare the external semaphore

// UI initialization functions
lv_obj_t* configMode_initUI(int rotation);
lv_obj_t* dataMonitor_initUI(int rotation);
lv_obj_t* dataMonitorCount_initUI(int rotation, uint8_t channel);
lv_obj_t* dataMonitorChart_initUI(int rotation, uint8_t channel);

extern uint8_t highLightChannel;

static void key_event_cb(lv_event_t * e);


#endif
