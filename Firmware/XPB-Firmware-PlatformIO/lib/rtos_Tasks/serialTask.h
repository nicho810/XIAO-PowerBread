/**
 * [INPUT]: Arduino.h, boardConfig.h, INA3221Sensor.h (DualChannelData)
 * [OUTPUT]: serialPrintTask() 任务函数
 * [POS]: 串口调试输出 (优先级1，最低)，human/plotter 双格式，可配置间隔
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include "INA3221Sensor.h"

void serialPrintTask(void *pvParameters);

#endif // SERIAL_TASK_H
