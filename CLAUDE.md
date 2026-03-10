# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

XIAO PowerBread is an embedded firmware for a breadboard power supply with real-time voltage/current/power monitoring. It uses Seeed Studio XIAO microcontrollers, an INA3221 power sensor, and a 0.96" ST7735S TFT LCD (80x160).

## Build Commands

All firmware lives in `Firmware/XPB-Firmware-PlatformIO/`. PlatformIO commands must run from that directory.

```bash
cd Firmware/XPB-Firmware-PlatformIO

# Build for a specific board (choose one environment)
pio run -e seeed_xiao_rp2040
pio run -e seeed_xiao_rp2350
pio run -e seeed_xiao_esp32c3
pio run -e seeed_xiao_esp32s3
pio run -e seeed_xiao_esp32c6

# Upload firmware
pio run -e seeed_xiao_rp2040 -t upload

# Serial monitor
pio device monitor -b 115200

# ESP32: merge binary for flashing
pio run -e seeed_xiao_esp32c3 -t mergebin

# Clean build
pio run -e seeed_xiao_rp2040 -t clean
```

No test framework is configured. There are no linting or formatting tools set up.

## Architecture

### Entry Point

`src/v2.0.0.cpp` — Initializes hardware (LCD, INA3221, dial encoder), sets up LVGL display driver, and spawns four FreeRTOS tasks.

### FreeRTOS Tasks (in `lib/rtos_Tasks/`)

| Task | Priority | Role |
|------|----------|------|
| `lvglTask` | 4 (highest) | LVGL rendering loop, display flush |
| `sensorUpdateTask` | 3 | Reads INA3221 every ~50ms, updates `latestSensorData` |
| `dialReadTask` | 2 | Rotary dial input, mode switching |
| `serialPrintTask` | 1 (lowest) | Optional serial debug output |

Tasks synchronize via semaphores (`lvglMutex`, `xSemaphore`, `configStateMutex`) and volatile global flags.

### Library Modules (in `lib/`)

- **boardConfig** — Pin definitions and platform-specific #ifdefs for all five XIAO boards. Board type is selected via `-D` build flag in `platformio.ini`.
- **INA3221Sensor** — Wraps the INA3221_RT library. Exposes `DualChannelData` struct (channel0/channel1, each with voltage/current/power).
- **sysConfig** — EEPROM-backed configuration (128 bytes). Handles struct versioning and migration. 11 user-configurable parameters.
- **lvgl_ui** — UI layer built on LVGL 8.3.x. Three display modes: `dataMonitor` (numeric), `dataMonitorChart` (line chart), `dataMonitorCount` (statistics). Config mode UI for EEPROM settings.
- **LGFX_096_XPB** — LovyanGFX hardware config for the ST7735S display. SPI bus/pin mapping varies per board.
- **dialSwitch** — Rotary encoder driver. Outputs dial events (up/down/press/longpress).
- **xpb_color_palette** — LVGL color constants for theming.

### Data Flow

```
INA3221 (I2C) → sensorUpdateTask → global DualChannelData
  → lvglTask (UI rendering)
  → serialPrintTask (optional debug)
```

### Multi-Board Support

Board selection is compile-time via build flags (`-D SEEED_XIAO_RP2040`, etc.). The `boardConfig.h` header uses `#ifdef` to set pin assignments, I2C/SPI instances, and FreeRTOS stack sizes per platform. RP2040/RP2350 use the earlephilhower Arduino core; ESP32 variants use the espressif32 platform.

### Key Dependencies

- **LVGL 8.3.4** — UI framework. Config header at `lv_conf/lv_conf.h`.
- **LovyanGFX 1.2.0** — SPI display driver (replaces Adafruit_GFX from v1.x).
- **INA3221_RT 0.4.0** — Power measurement sensor library.
- **FreeRTOS** — Built into both Arduino-Pico and Arduino-ESP32 cores.

### Legacy Firmware

`Firmware/XPB-Firmware-Arduino/` contains the v1.x Arduino IDE firmware (Adafruit_GFX based). It is not actively maintained. All new development targets the PlatformIO project.
