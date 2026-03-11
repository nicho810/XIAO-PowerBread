/**
 * [INPUT]: lvgl.h, sensorUpdateTask.h (EVT_*), lvgl_ui_updateFunc.h, xpb_display.h, sysConfig.h
 * [OUTPUT]: lvglTask() — UI 唯一控制者，事件驱动渲染 + widget 更新 + lv_timer_handler
 * [POS]: LVGL 渲染引擎 + UI 更新统一控制器 (优先级4)，持有 lvglMutex (无竞争)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#include "lvglTask.h"
#include "sensorUpdateTask.h"
#include "lvgl_ui_updateFunc.h"
#include "xpb_display.h"
#include "sysConfig.h"

extern ConfigMode configMode;
extern SemaphoreHandle_t sensorDataMutex;
extern DualChannelData latestSensorData;
extern lv_obj_t *ui_container;
extern volatile function_mode current_functionMode;
extern uint8_t highLightChannel;
extern volatile int tft_Rotation;
extern float avgS[2], avgM[2], avgH[2], peak[2];

/* sensorUpdateTask 向 lvglTask 发送的额外事件位 */
#define EVT_SENSOR_READY  (1 << 3)

void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    /* config mode UI 更新用的状态缓存 */
    static int8_t cfg_last_cursor = -1;
    static int8_t cfg_last_status = -1;

    /* UI 创建时的模式快照 — widget 更新用此值而非 volatile 全局，
     * 防止 key_event_cb 写 current_functionMode 但 EVT_MODE_CHANGE 尚未到达时的竞态 */
    static function_mode ui_built_for_mode = dataMonitor;

    /* DEBUG: 一次性诊断标志 */
    static bool dbg_started = false;
    static bool dbg_ui_created = false;
    static uint32_t dbg_loop_count = 0;

    while (1)
    {
        /* --- 接收事件 (非阻塞) --- */
        uint32_t events = 0;
        xTaskNotifyWait(0, 0xFFFFFFFF, &events, 0);

        /* DEBUG */
        if (!dbg_started) { Serial.println("[LVGL] task started"); dbg_started = true; }
        dbg_loop_count++;
        if (dbg_loop_count <= 20 || events != 0) {
            Serial.printf("[LVGL] #%lu evt=0x%lx ui=%p\n", dbg_loop_count, events, ui_container);
        }

        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
        {
            /* UI 尚未创建 → 仅 EVT_MODE_CHANGE 可触发首次创建，其余跳过 */
            if (!ui_container && !(events & EVT_MODE_CHANGE)) goto render;

            /* ============================================================
             *  Config Mode UI 更新
             * ============================================================ */
            if (configMode.configState.isActive)
            {
                lv_obj_t *item_area = lv_obj_get_child(ui_container, 1);
                if (configMode.configState.cursorStatus >= 0)
                {
                    if (cfg_last_cursor != configMode.configState.cursor ||
                        cfg_last_status != configMode.configState.cursorStatus)
                    {
                        update_configMode(item_area,
                                          configMode.configState.cursor,
                                          configMode.configState.cursorLast,
                                          configMode.configState.cursorMax,
                                          configMode.configState.cursorStatus);
                        cfg_last_cursor = configMode.configState.cursor;
                        cfg_last_status = configMode.configState.cursorStatus;
                    }

                    if (configMode.configState.cursorStatus == 1)
                    {
                        update_configMode_cfgData(item_area, configMode.configState.cursor);
                    }
                }
                else if (configMode.configState.cursorStatus == -1)
                {
                    configMode.configState.isActive = false;
                }
            }
            /* ============================================================
             *  Normal Mode UI 更新 (事件驱动)
             * ============================================================ */
            else
            {
                /* --- 模式切换：重建 UI --- */
                if (events & EVT_MODE_CHANGE)
                {
                    if (ui_container != NULL)
                    {
                        lv_obj_del(ui_container);
                        ui_container = NULL;
                    }
                    ui_built_for_mode = current_functionMode; // 快照：此刻 UI 对应的 mode
                    ui_container = xpb_display_create_ui(ui_built_for_mode, tft_Rotation, highLightChannel);
                    if (!dbg_ui_created) {
                        Serial.printf("[LVGL] UI created: %p mode=%d\n", ui_container, ui_built_for_mode);
                        dbg_ui_created = true;
                    }
                    events |= EVT_SENSOR_READY; // 模式切换后立即刷新数据
                }

                /* --- 数据更新 (copy-out + widget update) --- */
                if (events & (EVT_SENSOR_READY | EVT_FORCE_UPDATE))
                {
                    /* copy-out 传感器快照 (在 lvglMutex 内短暂取 sensorDataMutex) */
                    DualChannelData localData;
                    float localAvgS[2], localAvgM[2], localAvgH[2], localPeak[2];

                    if (xSemaphoreTake(sensorDataMutex, pdMS_TO_TICKS(2)) == pdTRUE)
                    {
                        localData = latestSensorData;
                        memcpy(localAvgS, avgS, sizeof(avgS));
                        memcpy(localAvgM, avgM, sizeof(avgM));
                        memcpy(localAvgH, avgH, sizeof(avgH));
                        memcpy(localPeak, peak, sizeof(peak));
                        xSemaphoreGive(sensorDataMutex);
                    }

                    /* --- widget 更新 --- */
                    lv_obj_t *container0 = lv_obj_get_child(ui_container, 0);
                    if (container0)
                    {
                        switch (ui_built_for_mode)
                        {
                        case dataMonitor:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, 0, localData);
                                update_monitor_data(container1, 1, localData);
                            }
                            break;

                        case dataMonitorChart:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, highLightChannel, localData);
                                float currentValue = (highLightChannel == 0)
                                    ? localData.channel0.busCurrent
                                    : localData.channel1.busCurrent;
                                update_chart_data(container1, currentValue);
                            }
                            break;

                        case dataMonitorCount:
                            if (events & EVT_HIGHLIGHT_CHANGE)
                            {
                                avgS[highLightChannel] = 0;
                                avgM[highLightChannel] = 0;
                                avgH[highLightChannel] = 0;
                                peak[highLightChannel] = 0;
                            }
                            update_monitor_data(container0, highLightChannel, localData);
                            {
                                lv_obj_t *c = lv_obj_get_child(ui_container, 1);
                                update_count_data(c, highLightChannel, localAvgS[highLightChannel]);
                                c = lv_obj_get_child(ui_container, 2);
                                update_count_data(c, highLightChannel, localAvgM[highLightChannel]);
                                c = lv_obj_get_child(ui_container, 3);
                                update_count_data(c, highLightChannel, localAvgH[highLightChannel]);
                                c = lv_obj_get_child(ui_container, 4);
                                update_count_data(c, highLightChannel, localPeak[highLightChannel]);
                            }
                            break;
                        }
                    }
                }
            }

        render:
            /* --- LVGL 渲染 --- */
            lv_timer_handler();

            lv_disp_t *disp = lv_disp_get_default();
            if (disp)
            {
                lv_refr_now(disp);
            }

            xSemaphoreGive(lvglMutex);
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
