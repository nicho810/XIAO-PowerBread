# boardConfig/
> L2 | 父级: lib/CLAUDE.md

## 成员清单

boardConfig.h: 编译时硬件抽象层，五板引脚定义 (LCD SPI / INA3221 I2C / Dial ADC)，平台特定 FreeRTOS 头文件路由，零实现文件

## 支持平台

| 宏定义 | I2C | SPI | Dial Pin |
|--------|-----|-----|----------|
| SEEED_XIAO_ESP32C3 | Wire(6,7) | SPI(8,10,9,3,4) | A0 |
| SEEED_XIAO_ESP32S3 | Wire(6,7) | SPI(8,10,9,3,4) | A0 |
| SEEED_XIAO_ESP32C6 | Wire(22,23) | SPI(21,19,18,20,1) | A0 |
| SEEED_XIAO_RP2040 | Wire1(7,6) | SPI(2,3,4,28,29) | A0 |
| SEEED_XIAO_RP2350 | Wire1(7,6) | SPI(2,3,4,28,29) | A0 |

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
