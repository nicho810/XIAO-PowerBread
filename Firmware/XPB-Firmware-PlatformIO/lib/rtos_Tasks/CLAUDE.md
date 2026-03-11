# rtos_Tasks/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

lvglTask.h/cpp: LVGL 渲染任务 (优先级4)，5ms 周期调用 lv_timer_handler + lv_refr_now，持有 lvglMutex
sensorUpdateTask.h/cpp: 传感器采样任务 (优先级3)，5ms 周期读 INA3221，EMA 均值计算 (avgS/avgM/avgH/peak)，选择性 UI 刷新，模式切换通过 xpb_display_create_ui()
dialReadTask.h/cpp: 旋钮读取任务 (优先级2)，100ms 周期，ADC→LVGL 键码转换，长按检测 (≥700ms→ESC)
serialTask.h/cpp: 串口输出任务 (优先级1)，可配置间隔，human/plotter 双格式

## 任务间通信

```
sensorUpdateTask ──(latestSensorData)──→ lvglTask (通过 update_* 函数 + lvglMutex)
dialReadTask ──(last_key)──→ keyboard_read() (通过 xSemaphore)
sensorUpdateTask ──(latestSensorData)──→ serialPrintTask (通过 xSemaphore)
sensorUpdateTask ←──(functionMode_ChangeRequested)──── key_event_cb (volatile flag)
```

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
