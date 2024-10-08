
## v1.1.3 (Stable)
### Improvements:

- Add fixed-scale and auto-scale support for line-chart. (Default is fixed-scale, max-scale is 500mA, This can be changed in sysConfig)
  
> Fixed-mode will be useful if the current is changing a lot (e.g. servo, motor). 

> Auto-mode will be useful if the current is stable (e.g. LED).

**Important Note:** Due to updates in the cfg structure, the EEPROM will be initialized to default values if the `cfg_version` does not match or is lower than `0x02`. Please be aware that this may reset your custom settings.

[System Config Explanation](Docs/sysConfig.md)



## v1.1.2 (Stable)
### Improvements:

- Debugging LED Blink: Added LED blink functionality to facilitate debugging processes.
- Code Refactoring: Restructured the source code for enhanced readability and comprehension.
- Customized Adafruit_ST7735 Library: Incorporated a modified version of the Adafruit_ST7735 library tailored to the specific LCD module used in this project.
- Default shunt resistor value is 20mR, if you are using different value resistor, please modify the value on sysConfig.

## v1.1.1 (Stable)
### Notes
1. A chart mode and current statistics display have been added to enhance visualization for each channel. You can access this mode by pressing the dial wheel.
2. The UI rotation can be adjusted in any mode by turning the dial wheel up or down.
3. To modify default settings, enter the settings mode by turning the dial down, holding it, and resetting/re-powering the device until the settings mode activates. Use the dial wheel to scroll through options (up/down) and press the dial briefly to select or exit. Long press the dial to save changes.
4. The default shunt resistor value is 20mΩ. If you're using a different shunt resistor, you can change the value in the settings mode (e.g., 20 = 20mΩ).

**For more detailed instructions, please refer to the README.**


## v1.1.0 (Beta) 
### Note
To improve visualization for each channel, a chart mode has been added. You can enter this mode by pressing the dial wheel, and switch back to the normal data mode by pressing the dial wheel again. When in chart mode, you can switch channels by turning the dial wheel up or down.

** This new release is not stable, and you might encounter some bugs. You can reset the XIAO if needed or switch back to the stable release. **




## 1.0.0 (Stable)
### Feature
- Real-time current sensor data displayed on the LCD.
- Dial wheel to adjust the UI for different viewing angles.