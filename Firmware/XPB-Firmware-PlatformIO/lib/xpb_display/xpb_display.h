/**
 * [INPUT]: lvgl.h, function_mode.h
 * [OUTPUT]: xpb_display_init(), xpb_display_create_ui(), xpb_display_create_config_ui(), XPB_SCREEN_*
 * [POS]: 显示子系统抽象层，封装 LCD 硬件 + LVGL 框架 + 输入设备，对外隐藏 LovyanGFX 细节
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#pragma once

#include <lvgl.h>
#include "function_mode.h"

/* ================================================================
 *  屏幕尺寸常量 — ST7735S 80x160
 * ================================================================ */
#define XPB_SCREEN_WIDTH  80
#define XPB_SCREEN_HEIGHT 160

/* ================================================================
 *  公开接口
 * ================================================================ */

// 初始化 LCD 硬件 + LVGL 框架 + 输入设备
// 内部创建 lvglMutex，调用前 xSemaphore 必须已就绪
bool xpb_display_init(int rotation);

// 统一 UI 创建入口，调用者必须持有 lvglMutex
// 内部分发到 dataMonitor_initUI / dataMonitorChart_initUI / dataMonitorCount_initUI
lv_obj_t* xpb_display_create_ui(function_mode mode, int rotation, uint8_t channel);

// Config mode UI 独立创建 (不属于 function_mode 枚举)
lv_obj_t* xpb_display_create_config_ui(int rotation);
