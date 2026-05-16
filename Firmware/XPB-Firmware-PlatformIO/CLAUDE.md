# XPB-Firmware-PlatformIO/
> L2 | 父级: /CLAUDE.md

## 成员清单

src/main.cpp: 系统入口，硬件初始化 + sensorDataMutex/keyboardMutex 创建 + FreeRTOS 四任务编排，Config Mode 启动逻辑
src/fonts/: LVGL 自定义字体 (Inter Bold 8px / ExtraBold 18px，1bit 和 2bit 变体)

lib/boardConfig/: 硬件抽象层，五板引脚定义 + FreeRTOS 头文件路由
lib/INA3221Sensor/: INA3221 传感器驱动，双通道数据采集 + isDirty 变化检测
lib/dialSwitch/: 旋钮编码器驱动，ADC 阈值判定 → 离散事件 (up/down/press)
lib/sysConfig/: EEPROM 持久化配置 (128字节)，版本迁移，ConfigMode 状态机
lib/xpb_display/: 显示子系统抽象层 — LCD + LVGL + 输入设备封装，统一 UI 创建入口，隐藏 LovyanGFX
lib/lvgl_ui/: UI 层，三种显示模式 + 配置模式，Widget 工厂 + 局部刷新
lib/xpb_protocol/: XPB Binary Protocol v1.2 引擎，CRC/帧构建/解析/三态会话状态机，零 I/O 依赖
lib/rtos_Tasks/: 四个 FreeRTOS 任务 (lvgl/sensor/dial/serial)，TaskNotify 事件驱动
lib/LGFX_096_XPB/: LovyanGFX 硬件配置，ST7735S 80x160 SPI 驱动
lib/xpb_color_palette/: LVGL 颜色常量，UI 主题单一真相源

docs/XPB_BINARY_PROTOCOL_V1.md: XPB 二进制串口协议 v1.2 规格文档，帧结构 + CRC + 三阶段握手 (START/CONFIG_ACK/STOP) + 会话生命周期 + 状态机 + STREAMING 重连 (任意状态接受 START)
debug/main.cpp: RP2040 硬件协议模拟器，Arduino setup/loop 范式，三状态状态机 (IDLE→WAIT_ACK→STREAMING)，USB-CDC Serial 输出 XPB Binary Protocol v1.2 帧流，RGB LED 状态指示 (红/蓝/绿)，零外部库依赖

lv_conf/lv_conf.h: LVGL 配置 — 16bit RGB565, 64KB 内存, 5ms tick
platformio.ini: 五板构建环境定义 + 依赖声明
merge-bin.py: ESP32 固件合并工具

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
