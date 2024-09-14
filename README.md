# XIAO PowerBread

XIAO PowerBread is an open-source hardware project designed to provide a versatile and efficient power solution for breadboard prototyping. With built-in sensors, real-time monitoring, and the power of the RP2040 microcontroller, XIAO PowerBread simplifies the development of electronics projects.

## Key Features

1. **Built-in INA3221 Multi-Channel Current Sensor**: Monitors voltage, current, and power consumption of the connected load electronics, providing detailed power metrics.
2. **High-Current Output**: Delivers up to 2A of 3.3V output for breadboard circuits, powered by the TI TPS54202 voltage regulator.
3. **Real-Time Power Monitoring**: Features a built-in LCD display that shows real-time voltage, current, and power information for two channels.
4. **Breadboard-Friendly Design**: Compact and easy to use, it plugs directly into a breadboard, providing two power outputs (5V and 3.3V).
5. **Open-Source and RP2040 Powered**: Fully open-source and driven by the RP2040 microcontroller, with plans to add USB-serial functionality and PWM generation in future updates.

## Hardware Specifications

- **Input Voltage**: Supports USB input or external DC power supply.
- **Output Voltages**: Provides both 5V and 3.3V, with a maximum 3.3V output current of 2A.
- **Current Sensing**: Uses the INA3221 sensor for real-time voltage, current, and power measurement.
- **Microcontroller**: Powered by the RP2040, enabling future extensions such as USB-serial communication and PWM output.
- **Display**: LCD for displaying real-time power statistics.
- **PCB Dimensions**: Designed for breadboard integration with minimal space usage.

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
