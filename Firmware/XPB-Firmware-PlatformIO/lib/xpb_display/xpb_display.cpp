/**
 * [INPUT]: LovyanGFX (LGFX_096_XPB), LVGL 8.3.4, lvgl_ui.h (UI 初始化函数)
 * [OUTPUT]: xpb_display_init(), xpb_display_create_ui(), xpb_display_create_config_ui()
 * [POS]: 显示子系统实现，LCD 硬件 + LVGL 驱动 + 输入设备注册均封装于此，tft 对外完全隐藏
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include "xpb_display.h"

#include <Arduino.h>
#include <LovyanGFX.h>
#include <LGFX_096_XPB.hpp>
#include "boardConfig.h"
#include "lvgl_ui.h"

/* 键盘状态 — 由 dialReadTask 拥有，此处仅读取 */
extern bool last_key_pressed;
extern uint32_t last_key;

/* ================================================================
 *  内部状态 — 对外完全隐藏
 * ================================================================ */
static LGFX tft;

/* ================================================================
 *  LVGL 显示 flush 回调
 * ================================================================ */
static void xpb_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t len = w * h;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((uint16_t *)&color_p->full, len, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

/* ================================================================
 *  LVGL 键盘输入回调
 * ================================================================ */
static void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        if (last_key_pressed)
        {
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = last_key;
        }
        else
        {
            data->state = LV_INDEV_STATE_RELEASED;
            data->key = last_key;
        }

        last_key_pressed = false;
        xSemaphoreGive(xSemaphore);
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
        data->key = 0;
    }
}

/* ================================================================
 *  外部信号量 — 由 main.cpp 创建，此处仅引用
 * ================================================================ */
extern SemaphoreHandle_t xSemaphore;

/* lvglMutex 属于显示子系统，在此定义 */
SemaphoreHandle_t lvglMutex = NULL;

/* ================================================================
 *  公开接口实现
 * ================================================================ */

bool xpb_display_init(int rotation)
{
    /* --- LCD 硬件初始化 --- */
    if (!tft.begin()) {
        Serial.println("LCD initialization failed!");
        return false;
    }
    tft.setColorDepth(16);
    tft.setRotation(rotation);
    tft.fillScreen(0x0000);

    /* --- LVGL 框架初始化 --- */
    lv_init();

    static lv_disp_draw_buf_t draw_buf;
    const int buf_size = XPB_SCREEN_WIDTH * 80;
    static lv_color_t buf1[XPB_SCREEN_WIDTH * 80];
    static lv_color_t buf2[XPB_SCREEN_WIDTH * 80];
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_size);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = XPB_SCREEN_WIDTH;
    disp_drv.ver_res = XPB_SCREEN_HEIGHT;
    disp_drv.flush_cb = xpb_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0;
    disp_drv.direct_mode = 0;
    disp_drv.antialiasing = 1;
    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);

    lv_disp_t *disp = lv_disp_get_default();
    disp->driver->monitor_cb = NULL;

    /* --- LVGL 输入设备注册 --- */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv);

    if (kb_indev)
    {
        lv_group_t *g = lv_group_create();
        lv_group_set_default(g);
        lv_indev_set_group(kb_indev, g);
    }

    /* --- 创建 lvglMutex --- */
    lvglMutex = xSemaphoreCreateMutex();

    return true;
}

lv_obj_t* xpb_display_create_ui(function_mode mode, int rotation, uint8_t channel)
{
    lv_obj_clean(lv_scr_act());
    switch (mode) {
        case dataMonitor:      return dataMonitor_initUI(rotation);
        case dataMonitorChart: return dataMonitorChart_initUI(rotation, channel);
        case dataMonitorCount: return dataMonitorCount_initUI(rotation, channel);
        default:               return dataMonitor_initUI(rotation);
    }
}

lv_obj_t* xpb_display_create_config_ui(int rotation)
{
    lv_obj_clean(lv_scr_act());
    return configMode_initUI(rotation);
}
