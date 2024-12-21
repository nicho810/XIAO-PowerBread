# Firmware Project of XIAO-PowerBread

## Overview

The firmware v1 is based on the Arduino IDE, while v2 will be developed using PlatformIO.

This change is due to the advanced AI capabilities of PlatformIO(with VSCode), which allow for more efficient firmware development despite increasing UI complexity. Let's say goodbye to the Arduino IDE.

## Platform Comparison

| Feature | PlatformIO | Arduino IDE |
|---------|--------------|-------------|
| Framework | Arduino | Arduino |
| Firmware Version | **2.0.0-Latest** | 0.9.0-1.1.4 |
| LCD Driver | LovyanGFX | Adafruit_GFX |
| UI Framework | LVGL v8.4 | None |
| Data Monitor | ✅ | ✅ |
| Data Monitor+Chart | ✅ | ✅ |
| Data Monitor+Count | ✅ | ✅ |
| Serial Monitor | ✅ | ✅ |
| UI Rotation | not planned | ✅ |
| PWM Output | ToDo | not planned |
| Analog Input Monitor | ToDo | not planned |


## Change Log

### v2.0.0

Improvements:
- Switch to PlatformIO from Arduino IDE, for improving the development experience.
- Switch to LovyanGFX from Adafruit_GFX, for improving the tft flush performance.
- Add LVGL v8.4, for making more complex UI in a efficient way.
- The LineChart is now powered by LVGL. Much better than the old one.

Known Issues:
- UI Rotation is not implemented yet. We plan to remove the UI rotation feature due to increasing UI complexity. It is preferable to use rotation operations within specific UIs like the Menu.
- System configuration is not implemented yet.

Dial wheel change:
- up and down: change the UI function.
- press: switch the channel(only trigger when in LineChart and data statistic mode).

*Note: this beta version is now only support XIAO RP2040. Other XIAO boards please use v1.1.4.*

*Note: Ensure you download the firmware version that matches the shunt resistor value for your specific XIAO board. Usually it is 50mOhm.*