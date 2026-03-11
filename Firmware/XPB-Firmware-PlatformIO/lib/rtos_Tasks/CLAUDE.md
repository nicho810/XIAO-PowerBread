# rtos_Tasks/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

lvglTask.h/cpp: UI 唯一控制者 (优先级4)，事件驱动 widget 更新 + config mode UI + 模式切换 + lv_timer_handler，持有 lvglMutex (无竞争)
sensorUpdateTask.h/cpp: 纯数据生产者 (优先级3)，5ms 周期 I2C 采集 + EMA 计算，零 LVGL 依赖，定义 EVT_* 事件位
dialReadTask.h/cpp: 旋钮读取任务 (优先级2)，100ms 周期，ADC→LVGL 键码转换，长按检测 (≥700ms→ESC)，通过 keyboardMutex 保护写入
serialTask.h/cpp: 串口输出任务 (优先级1)，可配置间隔，human/plotter 双格式，通过 sensorDataMutex 读取数据

## 任务间通信

```
key_event_cb ──TaskNotify(EVT_MODE/HIGHLIGHT_CHANGE)──→ sensorUpdateTask
sensorUpdateTask ──TaskNotify(EVT_SENSOR_READY + 转发 EVT_*)──→ lvglTask
lvglTask ──copy-out(sensorDataMutex)──→ localData → update_* widgets
dialReadTask ──(last_key)──→ keyboard_read() (通过 keyboardMutex)
serialPrintTask ──(latestSensorData)──← sensorUpdateTask (通过 sensorDataMutex)
```

## 锁层级

lvglMutex(L1, 仅 lvglTask) → sensorDataMutex(L2) → keyboardMutex(L3) → configStateMutex(L4)

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
