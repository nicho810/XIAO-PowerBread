# XIAO PowerBread - A Breadboard Power Supply with Real-Time Monitoring

English | [Deutsch](README_DE.md) | [Français](README_FR.md) | [日本語](README_JP.md)

**XIAO PowerBread** is an open-source hardware project designed to offer a reliable and efficient power solution for breadboard prototyping. Featuring built-in sensors, real-time monitoring, and compatibility with [Seeed Studio XIAO microcontrollers](https://www.seeedstudio.com/xiao-series-page), PowerBread makes powering and developing electronics projects easier than ever.

![XIAO PowerBread](Docs/Images/pic_overview.png)
> Easy installation and compact design

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.png)
> Direct power and monitoring you breadboard projects

## Key Features

1. **Real-Time Monitoring**: View essential power metrics at a glance—no multimeter needed. The built-in display shows real-time voltage, current, and power data.
2. **High-Current Output**: Deliver up to 1.5A of 3.3V power, ideal for most breadboard-based electronics projects.
3. **Built-in LCD Display**: Stay informed with real-time feedback—critical power data is displayed clearly on the integrated LCD.
4. **Plug-and-Play Design**: Compatible with standard breadboards, simply plug in, power up, and start prototyping without extra setup.
5. **Open-Source and Seeed Studio XIAO Compatibility**: Compatible with almost all XIAO Dev Boards (the support for XIAO SAMD21 and XIAO RA4M1 is still on the way), this open-source design provides flexibility for future extensions like USB-serial communication and PWM control.
6. **Dual-Channel Voltage and Current Sensing**: Equipped with the INA3221 sensor for dual-channel voltage and current monitoring—know what’s happening with every aspect of your power supply.
7. **Compact Powerhouse**: This compact design delivers both 3.3V and 5V outputs, optimizing your breadboard space without compromising on power.
8. **Multiple UI functions**: The UI can be switched between data monitoring, line chart, and statistic mode.

![function](Docs/Images/pic_functions.png)
> Multiple UI functions

![switchCH](Docs/Images/pic_switchCH.png)
> Long press the dial to switch channels in chart and statistic mode

![uiRotate](Docs/Images/pic_uiRotation.png)
> Rotate the dial to adjust the UI for different viewing angles


## Hardware Specifications

- **Input Voltage**: Powered via USB-C through the Seeed Studio XIAO.
- **Output Voltages**: Provides 5V and 3.3V outputs, with a maximum current of 1.5A for the 3.3V rail.
- **Current Sensing**: The built-in INA3221 sensor measures voltage, current, and power for real-time display.
- **Microcontroller**: Driven by the RP2040, handling sensor data, display control, and enabling future features like USB-serial communication and PWM generation.
- **Display**: LCD screen for real-time power monitoring.
- **PCB Dimensions**: Designed for seamless integration with standard breadboards while minimizing space usage.

![power path](Docs/Images/pic_outputSwitch.png)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.png)

## Getting Started

### Hardware

- **XIAO Board**: The [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) is highly recommended, also you can use: [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html), [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html), [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html), [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html). (Make sure to flash the firmware compatible with your board type)
- **XIAO PowerBread Board**: Clone or remix the design from this repository. Also you can get the hardware from [Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) or [Tindie](https://www.tindie.com/products/35842/).
- **Breadboard**: Fits standard breadboards for easy prototyping.
- **Power Supply**: Use a standard USB power source.


### Get the Hardware

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html">
          <img src="Docs/Images/seeed-logo.png" width="200"><br>
          <b>Seeed Studio</b><br>
          Get the Kit and XIAO board
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/">
          <img src="Docs/Images/tindie-logo.png" width="200"><br>
          <b>Tindie</b><br>
          Get the fully assembled hardware
        </a>
      </td>
    </tr>
  </table>
</div>

### Software

- **Remix the project**: Modify or remix the project using the provided Arduino source code.
- **Directly use compiled firmware**: 
  - For XIAO ESP32 series
    - Web based flash tool: https://powerbread-flasher.ioatlas.com
    - Tutorial: [Flash Firmware for XIAO - ESP32 series](Docs/flash-firmware-for-esp32-series.md)
  - For XIAO RP2040/RP2350
    - Flash tool: Use the UF2 method to upload the firmware:
      1. Download the firmware file (*.uf2) from the [Release page](https://github.com/nicho810/XIAO-PowerBread/releases)
      2. Connect XIAO to your computer via USB cable (must be a 4-pin data cable)
      3. Enter bootloader mode:
         - Press and hold the BOOT button (B)
         - While holding BOOT, press and release the RESET button (R)
         - Release the BOOT button
      4. A new USB drive named "RPI-RP2" will appear on your computer
      5. Drag and drop the .uf2 firmware file onto the RPI-RP2 drive
      6. The XIAO will automatically reset and start running the new firmware



### Planned Software Features
- [x] Real-time current sensor data displayed on the LCD.
- [x] Dial wheel to adjust the UI for different viewing angles.
- [x] Line chart mode to visualize power usage for each channel.
- [x] Count mode to count average(second, minute, all time) and peak current consumption for each channel.
- [x] Allow to save settings data to EEPROM.
- [ ] USB to Serial mode as a debug tool.
- [ ] PWM output on IO0 and IO1.
- [ ] ADC reading from IO0 and IO1.
- [ ] Web based UI for data monitoring.

### Firmware Releases

| Version | Stable | Development | Features added | Link |
|---------|---------|---------|---------|---------|
| 0.9.0     | Yes | Yes | Real-time current sensor data displayed on the LCD. | - |
| 1.0.0     | Yes | Yes | Dial wheel to adjust the UI for different viewing angles. | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0     | No  | Yes | Line chart mode to visualize power usage for each channel. | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1     | Yes  | Yes | support Data-Dashboard, Line-chart, current-Statistic，Setting | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2     | Yes | Yes | Introduces LED blink functionality for debugging purposes | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3     | Yes👍 | Yes | Add fixed-scale and auto-scale support for line-chart | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4     | Yes👍 | Yes | Add support for XIAO RP2350, XIAO ESP32-C3, XIAO ESP32-S3, XIAO ESP32-C6 | [Release page](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |

### Documentation
- [System Config Explanation](Docs/sysConfig.md)
  - Default mode setting
  - Shunt resistor setting (20mOhm, 50mOhm)
  - Serial output setting (Human readable mode,  Arduino plotter mode)
  - Line chart setting (Chart refresh rate)

### Troubleshooting
1. The display is not responding
   - Pls make sure you are using the latest firmware, the latest firmware have some bug fix for the stability.
   - Try to reset the XIAO by pressing the reset button on the XIAO board.
2. The display is not working
   - Please check the I2C and SPI pin connections, and ensure the XIAO is properly soldered the pins.
3. The current value is not correct
   - Pls check the shunt resistor value, and set it correctly in the sysConfig. (e.g. 20 for 20mOhm, 50 for 50mOhm) -> [System Config Explanation](Docs/sysConfig.md)
4. Both of the Volatage shows on display is 0V
   - Pls check the power-on switch, it should be on the down side. (UP side is OFF, DOWN side is ON)


### Libraries Used
- [INA3221_RT Library](https://github.com/RobTillaart/INA3221_RT/tree/master)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735 Library](https://github.com/adafruit/Adafruit-ST7735-Library) 
- [Arduino-Pico Core (4.x.x)](https://github.com/earlephilhower/arduino-pico)
- [Arduino-ESP32 Core (3.x.x)](https://github.com/espressif/arduino-esp32)
- [adafruit sleepydog](https://github.com/adafruit/Adafruit_SleepyDog)

> Included a modified version of Adafruit_ST7735 library since v1.1.2 to fit the lcd module used on this project.

## Contributing

Contributions are welcome to improve the XIAO PowerBread project! Whether you want to submit a pull request, propose new features, or report a bug, feel free to use the issue tracker.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.
