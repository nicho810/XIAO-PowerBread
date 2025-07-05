#ifndef LVGL_UI_MANAGER_H
#define LVGL_UI_MANAGER_H


#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "currentSensor.h"
#include "lvgl_ui_mode.h"
#include "lvgl_keyboard.h"
#include "lvgl_ui_widget_monitor.h"
#include "lvgl_ui_widget_chart.h"
// #include "lvgl_ui_widget_count.h"
// #include "lvgl_ui_widget_config.h"







// UI manager class
class UI_manager{
    private:
        UI_Mode current_UI_mode;
        UI_Mode previous_UI_mode;
        Widget_DataMonitor* dataMonitor_1;
        Widget_DataMonitor* dataMonitor_2;
        Widget_DataChart* dataChart_1;
        Widget_DataChart* dataChart_2;

    public:
        UI_manager();
        ~UI_manager();
        void initUI(UI_Mode mode = UI_Mode_DataMonitor, lv_obj_t* container = NULL);
        void updateUI(UI_Mode mode, SensorDataMessage sensorDataMessage, lv_obj_t* container = NULL);
        void switch_UI(UI_Mode mode);
        void switch_UI_next(); // switch to next UI mode
        void switch_UI_prev(); // switch to previous UI mode

        UI_Mode getCurrentMode() const { return current_UI_mode; }
        UI_Mode getPreviousMode() const { return previous_UI_mode; }
        void setPreviousMode(UI_Mode mode) { previous_UI_mode = mode; }
};



#endif