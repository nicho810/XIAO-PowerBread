#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "lvgl_ui_widget_monitor.h"
#include "currentSensor.h"
// #include "lvgl_func.h"
// #include "lvgl_ui_widget_chart.h"
// #include "lvgl_ui_widget_count.h"
// #include "lvgl_ui_widget_config.h"

extern SemaphoreHandle_t lvglMutex;

//UI Mode enum
enum UI_Mode{
    UI_Mode_Config,
    UI_Mode_DataMonitor,
    UI_Mode_DataCount,
    UI_Mode_DataChart,
};

// UI manager class
class UI_manager{
    private:
        UI_Mode current_UI_mode;
        Widget_DataMonitor* dataMonitor_A;
        Widget_DataMonitor* dataMonitor_B;

    public:
        UI_manager();
        ~UI_manager();
        void initUI(UI_Mode mode = UI_Mode_DataMonitor, lv_obj_t* container = NULL);
        void updateUI(UI_Mode mode, SensorDataMessage sensorDataMessage, lv_obj_t* container = NULL);
        void switch_UI(UI_Mode mode);

        UI_Mode getCurrentMode() const { return current_UI_mode; }
};








// #include "function_mode.h"
// #include "configMode.h"
// #include "currentSensor.h"
// #include "lvgl_ui_updateFunc.h"

// Global variables
// extern volatile bool menu_is_visible;
// extern volatile bool functionMode_ChangeRequested;
// extern volatile bool highLightChannel_ChangeRequested;
// extern volatile function_mode current_functionMode;
// extern uint8_t highLightChannel;



// // Generic UI initialization function
// void initUI(function_mode mode, lv_obj_t* container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);
// void updateUI(function_mode mode, lv_obj_t* container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);

// // UI update function type definition
// typedef void (*ui_update_func)(lv_obj_t* container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);


// // Structure to hold UI update functions for each mode
// struct UIModeHandlers {
//     ui_update_func init_handler;
//     ui_update_func update_handler;
// };

// // Declare the handlers array
// extern const UIModeHandlers UI_MODE_HANDLERS[];

// // UI initialization functions
// lv_obj_t* configMode_initUI(int rotation);
// void dataMonitor_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);
// void dataMonitorCount_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);
// void dataMonitorChart_initUI(lv_obj_t *ui_container, uint8_t channel, DualChannelData newSensorData, float floatValue, int32_t intValue);

// static void key_event_cb(lv_event_t * e);


#endif
