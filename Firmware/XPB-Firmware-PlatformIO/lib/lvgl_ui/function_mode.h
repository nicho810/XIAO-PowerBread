/**
 * [INPUT]: 无
 * [OUTPUT]: function_mode 枚举 (dataMonitor / dataMonitorChart / dataMonitorCount)
 * [POS]: UI 模式定义，被 lvgl_ui 和 rtos_Tasks 共同引用的类型契约
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef FUNCTION_MODE_H
#define FUNCTION_MODE_H

enum function_mode
{
    dataMonitor,
    dataMonitorChart,
    dataMonitorCount,
    // serialMonitor,
    // pwmOutput,
    // analogInputMonitor,
};



#endif // FUNCTION_MODE_H