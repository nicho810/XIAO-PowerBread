#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "currentSensor.h"
#include "lvgl_ui_widget_monitor.h"
#include "lvgl_ui_widget_chart.h"
// #include "lvgl_ui_widget_count.h"
// #include "lvgl_ui_widget_config.h"
// #include "lvgl_func.h"

extern SemaphoreHandle_t lvglMutex;



#if (currentSensor_TotalChannel==1)
enum UI_Mode{
    UI_Mode_Config,
    UI_Mode_DataMonitor,
    UI_Mode_DataCount,
    UI_Mode_DataChart,
};
#elif (currentSensor_TotalChannel==2)
enum UI_Mode{
    UI_Mode_Config,
    UI_Mode_DataMonitor,
    UI_Mode_DataCount_1,
    UI_Mode_DataCount_2,
    UI_Mode_DataChart_1,
    UI_Mode_DataChart_2,
};
#elif (currentSensor_TotalChannel==3)
enum UI_Mode{
    UI_Mode_Config,
    UI_Mode_DataMonitor,
    UI_Mode_DataCount_1,
    UI_Mode_DataCount_2,
    UI_Mode_DataCount_3,
    UI_Mode_DataChart_1,
    UI_Mode_DataChart_2,
    UI_Mode_DataChart_3,
};
#elif (currentSensor_TotalChannel==4)
enum UI_Mode{
    UI_Mode_Config,
    UI_Mode_DataMonitor,
    UI_Mode_DataCount_1,
    UI_Mode_DataCount_2,
    UI_Mode_DataCount_3,
    UI_Mode_DataCount_4,
    UI_Mode_DataChart_1,
    UI_Mode_DataChart_2,
    UI_Mode_DataChart_3,
    UI_Mode_DataChart_4,
};
#endif


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

        UI_Mode getCurrentMode() const { return current_UI_mode; }
        UI_Mode getPreviousMode() const { return previous_UI_mode; }
        void setPreviousMode(UI_Mode mode) { previous_UI_mode = mode; }
};


#endif
