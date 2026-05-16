/**
 * [INPUT]: Arduino.h, boardConfig.h, dialSwitch.h, lvgl.h, keyboardMutex (extern)
 * [OUTPUT]: dialReadTask() 任务函数, update_keyboard_state(), last_key/last_key_pressed (extern)
 * [POS]: 旋钮输入任务 (优先级2)，100ms 周期采样，长按检测 (≥700ms→ESC)，通过 keyboardMutex 保护键盘状态写入
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef DIALREADTASK_H
#define DIALREADTASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include "dialSwitch.h"
#include "lvgl.h"

// External declarations
extern SemaphoreHandle_t keyboardMutex;
extern DialFunction dial;
extern volatile int dialStatus;
extern volatile int lastDialStatus;

// Make keyboard state variables accessible
extern bool last_key_pressed;
extern uint32_t last_key;

// Function declaration
void dialReadTask(void *pvParameters);
void update_keyboard_state(uint8_t dialStatus);

#endif