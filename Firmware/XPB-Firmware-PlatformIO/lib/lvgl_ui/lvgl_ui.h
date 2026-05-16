/**
 * [INPUT]: Arduino.h, boardConfig.h, lvgl.h, xpb_color_palette.h, lvgl_ui_widget.h, function_mode.h, sensorUpdateTask.h
 * [OUTPUT]: 4种 UI 初始化函数, key_event_cb (internal, 通过 TaskNotify 发送 EVT_* 事件)
 * [POS]: UI 场景图构建入口 + LVGL 键盘事件路由，key_event_cb 零锁依赖 (消灭 ABBA 死锁)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "xpb_color_palette.h"
#include "lvgl_ui_widget.h"
#include "function_mode.h"

// External declarations for global variables
extern volatile function_mode current_functionMode;
extern uint8_t highLightChannel;
extern TaskHandle_t xSensorTaskHandle;

lv_obj_t* dataMonitor_initUI(int rotation);
lv_obj_t* dataMonitorCount_initUI(int rotation, uint8_t channel);
lv_obj_t* dataMonitorChart_initUI(int rotation, uint8_t channel);
lv_obj_t* configMode_initUI(int rotation);

// key_event_cb 仅 lvgl_ui.cpp 内部使用，不再在 header 声明


#endif
