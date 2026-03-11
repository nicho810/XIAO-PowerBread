# XIAO PowerBread - 面包板电源供应器，实时电压/电流/功率监控
PlatformIO + LVGL 8.3.4 + LovyanGFX 1.2.0 + INA3221_RT 0.4.0 + FreeRTOS

<directory>
Firmware/XPB-Firmware-PlatformIO/ - 主固件工程 (PlatformIO 构建)
  src/              - 入口点 main.cpp + 自定义字体
  lib/              - 8 个功能模块 (boardConfig, INA3221Sensor, dialSwitch, sysConfig, lvgl_ui, rtos_Tasks, LGFX_096_XPB, xpb_color_palette)
  lv_conf/          - LVGL 配置头文件
Firmware/[Archived] XPB-Firmware-Arduino.zip - v1.x 旧固件归档 (不再维护)
</directory>

<config>
platformio.ini - 五板构建环境 (RP2040/RP2350/ESP32-C3/S3/C6)，依赖声明，编译标志
lv_conf/lv_conf.h - LVGL 运行时配置：16bit RGB565，64KB 内存池，5ms 刷新
merge-bin.py - ESP32 固件合并脚本
</config>

## Build

```bash
cd Firmware/XPB-Firmware-PlatformIO
pio run -e seeed_xiao_rp2040       # 构建
pio run -e seeed_xiao_rp2040 -t upload  # 烧录
pio device monitor -b 115200       # 串口监视
pio run -e seeed_xiao_esp32c3 -t mergebin  # ESP32 合并二进制
```

## Architecture

### 数据流

```
INA3221 (I2C) → sensorUpdateTask (5ms) → latestSensorData (全局共享)
  ├→ sensorUpdateTask (取 lvglMutex) → update_* → LVGL widgets
  ├→ lvglTask (5ms) → lv_timer_handler → LovyanGFX → ST7735S LCD
  └→ serialPrintTask → Serial (可选调试)

Dial (ADC) → dialReadTask (100ms) → last_key → keyboard_read() → LVGL input → key_event_cb
```

### FreeRTOS 任务

| 任务 | 优先级 | 职责 |
|------|--------|------|
| lvglTask | 4 | LVGL 渲染循环，display flush |
| sensorUpdateTask | 3 | INA3221 采样，滚动均值计算，选择性 UI 刷新 |
| dialReadTask | 2 | 旋钮输入，模式切换，长按检测 |
| serialPrintTask | 1 | 串口调试输出 (human / Arduino Plotter) |

### 同步原语

- `lvglMutex` — 保护所有 LVGL 操作
- `xSemaphore` — 保护 latestSensorData + 键盘状态
- `configStateMutex` — 保护 ConfigMode 状态机

### 多板支持

编译时 `-D SEEED_XIAO_<VARIANT>` 选择目标板。`boardConfig.h` 用 `#ifdef` 统一引脚/I2C/SPI/FreeRTOS 栈大小。RP2040/RP2350 用 earlephilhower core；ESP32 用 espressif32 platform。

### UI 模式

- `dataMonitor` — 双通道数值显示 (V/I/P)
- `dataMonitorChart` — 单通道数值 + 折线图
- `dataMonitorCount` — 单通道数值 + 滚动均值 (1s/1m/1h) + 峰值
- `configMode` — EEPROM 配置编辑 (11 项参数)

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
