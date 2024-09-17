# XIAO PowerBread

XIAO PowerBread is an open-source hardware project designed to provide a versatile and efficient power solution for breadboard prototyping. With built-in sensors, real-time monitoring, and the power of the RP2040 microcontroller, XIAO PowerBread simplifies the development of electronics projects.

![XIAO PowerBread](Images/pic_overview.png)


## Key Features

1. **Real-Time Monitoring**: Direct monitoring, no multimeter needed – easily see what’s powering your project in real time.
2. **High-Current Output**: Power your breadboard with confidence – up to 1.5A of steady 3.3V output!
3. **Built-in LCD Display**: Instant feedback at a glance – real-time voltage, current, and power displayed in one place.
4. **Plug-and-Play Design**: Plug in, power up, and go – breadboard-friendly and ready to work.
5. **Open-Source and RP2040 Powered**: Flexible, open-source, and RP2040-powered – ready to grow with your projects.
6. **Dual-Channel Volatge and Current Sensor**: Monitor every detail – Dual sensing gives you the full picture.
7. **Compact Powerhouse**: Small form, big power – optimize your breadboard setup with 3.3V and 5V outputs.

![XIAO PowerBread](Images/pic_directPowerAndMonitoring.png)

## Hardware Specifications
- **Input Voltage**: USB-C input by XIAO.
- **Output Voltages**: Provides both 5V and 3.3V, with a maximum 3.3V output current of 1.5A.
- **Current Sensing**: Uses the INA3221 sensor for real-time voltage, current, and power measurement.
- **Microcontroller**: Powered by the RP2040, driving the current sensor and LCD, also enabling future extensions such as USB-serial communication and PWM output.
- **Display**: LCD for displaying real-time power statistics.
- **PCB Dimensions**: Designed for breadboard integration with minimal space usage.

![XIAO PowerBread](Images/pic_hardwareSpec.png)


## Getting Started

### Prerequisites

- **RP2040 Microcontroller**: Ensure you have the necessary software tools for programming the RP2040.
- **Breadboard**: The PowerBread is designed to fit standard breadboards.
- **Power Supply**: Use USB or external DC power for operation.

### Installation

1. **Download the Repository**:
   Clone or download the XIAO PowerBread repository to access the PCB files, firmware examples, and BOM.

   ```bash
   git clone https://github.com/nicho810/XIAO-PowerBread.git
   ```

2. **Assemble the Hardware**:
   Follow the instructions in the provided documentation to assemble the XIAO PowerBread. All necessary components are listed in the BOM.

3. **Programming the RP2040**:
   Use the Arduino IDE or other compatible environments to program the RP2040 with the provided firmware or your own custom code.

### Usage

1. **Power the Board**:
   Connect the PowerBread to USB or an external power source.

2. **Real-Time Monitoring**:
   View real-time voltage, current, and power consumption on the LCD display for two channels.

3. **Extend Functionality**:
   In future updates, you can use the RP2040 for additional functions like USB-serial communication or PWM generation.

## Example Projects

- **Monitoring Power Consumption**: Use the built-in INA3221 sensor to monitor power usage of different breadboard circuits in real-time.
- **Custom Control**: Program the RP2040 to manage power output or integrate with other sensors and actuators.

## Contributing

We welcome contributions to enhance the XIAO PowerBread project! Feel free to submit pull requests, suggest new features, or report bugs via the issue tracker.

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

---

Feel free to edit further based on additional functionality or details.
