# lvgl_ui/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

function_mode.h/cpp: function_mode 枚举定义 (dataMonitor / dataMonitorChart / dataMonitorCount)
lvgl_ui.h/cpp: 4种 UI 场景初始化函数，LVGL 键盘事件回调 key_event_cb，模式切换 + 旋转处理
lvgl_ui_widget.h/cpp: Widget 工厂 — DataMonitor (78x78 V/I/P)，DataChart (折线图 70点)，DataCount (均值标签)，configMode_item (配置行)
lvgl_ui_updateFunc.h/cpp: 局部刷新函数 — update_monitor_data / update_chart_data / update_count_data / update_chart_range / update_configMode

## UI 初始化函数

```cpp
dataMonitor_initUI(rotation)        → 双通道数值面板
dataMonitorChart_initUI(rotation, ch) → 单通道数值 + 折线图
dataMonitorCount_initUI(rotation, ch) → 单通道数值 + 滚动均值/峰值
configMode_initUI(rotation)         → 11项 EEPROM 参数编辑
```

## 旋转支持

- Portrait (80x160): 两个 78x78 widget 纵向堆叠
- Landscape (160x80): 两个 widget 横向并列

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
