/**
 * [INPUT]: Arduino.h, boardConfig.h, INA3221Sensor.h (DualChannelData), sysConfig.h (SysConfig/ConfigMode), xpb_protocol.h (XpbFrameParser/XpbSession), sensorDataMutex (extern)
 * [OUTPUT]: serialPrintTask() 任务函数
 * [POS]: 双模串口任务 (优先级1): IDLE 时文本调试 (human/plotter)，收到 START 自动切二进制协议流 (100Hz)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include "INA3221Sensor.h"

void serialPrintTask(void *pvParameters);

#endif // SERIAL_TASK_H
