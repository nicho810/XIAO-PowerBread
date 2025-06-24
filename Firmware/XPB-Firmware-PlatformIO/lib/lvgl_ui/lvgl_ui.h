#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "currentSensor.h"
#include "lvgl_ui_widget_monitor.h"
#include "lvgl_func.h"
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


#endif
