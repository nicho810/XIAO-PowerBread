# System Config Explanation


### Enter config mode
- When the dial switch is turn to the down position when the code starts, the code will enter the config mode. So you can turn the dial switch to the down position, hold it and reset the board or re-power the board.

### Change config
- After entering the config mode, you can use the dial switch to adjust the config cursor. (up/down)
- short press the dial switch to select the config item.
- after enter the config item, you can use the dial switch (up/down) to adjust the config value.
- after adjust the config value, you need to short press the dial switch again to exit the selection.

> **after exit the selection, the config still not saved, you need to long press the dial switch to save the config and exit the config mode.**

### Exit config mode
- Long press the dial switch to save the config and exit the config mode.
- if you want to exit the config mode without saving the config, you can directly reset the board or re-power the board.


## System Configuration Parameters

| Parameter | UI text | Type | Address | Default | Range (min:max) | Description |
|-----------|---------|------|---------|---------|-----------------|-------------|
| cfg_version | N/A | uint8_t | 0 | 1 | N/A | Increment when the config struct is changed |
| default_mode | Dft Mode | uint8_t | 1 | 0 | 0:2 | Default operation mode (0: dataMonitor, 1: dataMonitorChart, 2: dataMonitorCount) |
| default_channel | Dft CH | uint8_t | 2 | 0 | 0:1 | Default channel selection (0: CHA, 1: CHB) |
| shuntResistorCHA | ShuntR A | uint8_t | 3 | 20 | 0:255 | Shunt resistor value for Channel A (default: 20mOhm, 20=20mOhm) |
| shuntResistorCHB | ShuntR B | uint8_t | 4 | 20 | 0:255 | Shunt resistor value for Channel B (default: 20mOhm, 20=20mOhm) |
| serial_enable | UART EN | uint8_t | 5 | 1 | 0:1 | Serial communication enable/disable (0: disable, 1: enable) |
| serial_baudRate | UART BR | uint8_t | 6 | 0 | 0:0 | Serial communication baud rate (0: 115200, currently only 115200 is supported) |
| serial_mode | UART Mode | uint8_t | 7 | 0 | 0:1 | Serial communication mode (0: default/human readable, 1: arduino plotter mode) |
| serial_printInterval | UART IT | uint8_t | 8 | 0 | 0:4 | Interval for serial printing (0: 1000ms, 1: 500ms, 2: 100ms, 3: 50ms, 4: 10ms) |
| chart_updateInterval | Chart IT | uint8_t | 9 | 0 | 0:4 | Interval for chart updates (0: 50ms, 1: 100ms, 2: 250ms, 3: 500ms, 4: 1000ms) |


> **The config value is saved on the EEPROM, but the RP2040 does not have an EEPROM; instead, it uses the spi-flash to emulate the EEPROM. Please avoid writing the config value too frequently, as this can reduce the flash memory's lifespan. In this project, we only use it to save the config, so it's not a problem.**

## Demo Video
[Setting demo](Docs/Videos/Setting%20demo.mov)
