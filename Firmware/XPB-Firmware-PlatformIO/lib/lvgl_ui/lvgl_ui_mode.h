#ifndef LVGL_UI_MODE_H
#define LVGL_UI_MODE_H


#include "Arduino.h"
#include "boardConfig.h"


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


#endif