# lib/
> L2 | 父级: XPB-Firmware-PlatformIO/CLAUDE.md

9 个功能模块，职责单一，通过 TaskNotify + 细粒度信号量跨任务通信。

## 模块清单

### 硬件抽象层
boardConfig/boardConfig.h: 编译时平台选择，五板引脚映射 + FreeRTOS 头文件路由，零运行时开销

### 传感器
INA3221Sensor/: INA3221 双通道电压/电流/功率采集，isDirty 脏标志驱动选择性 UI 刷新

### 输入
dialSwitch/: 旋钮 ADC → 离散事件转换，平台特定阈值，消抖逻辑

### 持久化
sysConfig/: EEPROM 128字节配置存储，11项参数，版本迁移，ConfigMode 状态机 (信号量保护)

### 显示
xpb_display/: 显示子系统抽象层 — LCD 初始化 + LVGL 框架 + 输入设备 + 统一 UI 创建入口，隐藏 LovyanGFX
LGFX_096_XPB/: LovyanGFX ST7735S 硬件配置，80MHz SPI，平台自适应总线选择
xpb_color_palette/: 7 个 LVGL 颜色常量，双通道色彩体系 (cyan/magenta)
lvgl_ui/: UI 场景图构建 + Widget 工厂 + 局部刷新 + 输入事件路由，4种模式

### 任务调度
rtos_Tasks/: 4 个 FreeRTOS 静态任务，优先级 4→1，TaskNotify 事件驱动 + 细粒度信号量协同

## 依赖关系

```
boardConfig ← (所有模块依赖)
INA3221Sensor ← boardConfig, Wire, INA3221_RT
dialSwitch ← boardConfig
sysConfig ← boardConfig, EEPROM
LGFX_096_XPB ← boardConfig, LovyanGFX
xpb_color_palette ← LVGL
lvgl_ui ← xpb_color_palette, INA3221Sensor, sysConfig, LVGL
xpb_display ← LGFX_096_XPB, lvgl_ui, boardConfig, LVGL
rtos_Tasks ← xpb_display, lvgl_ui, INA3221Sensor, dialSwitch, sysConfig
```

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
