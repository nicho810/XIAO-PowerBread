#ifndef sysConfig_configData_h
#define sysConfig_configData_h

#include <Arduino.h>


// Configuration version constants
static constexpr uint8_t CONFIG_VERSION_UNINITIALIZED = 0xFF;
static constexpr uint8_t CONFIG_VERSION_CURRENT = 2;


// Configuration limits
static constexpr uint8_t MAX_MODE = 2;
static constexpr uint8_t MAX_CHANNEL = 1;
static constexpr uint8_t MAX_SHUNT_RESISTOR = 255;
static constexpr uint8_t MAX_SERIAL_BAUDRATE = 0;
static constexpr uint8_t MAX_SERIAL_MODE = 1;
static constexpr uint8_t MAX_SERIAL_INTERVAL = 4;
static constexpr uint8_t MAX_CHART_INTERVAL = 4;
static constexpr uint8_t MAX_CHART_SCALE_MODE = 1;
static constexpr uint8_t MAX_CHART_SCALE = 255;
static constexpr uint8_t MIN_CHART_SCALE = 1;

// Configuration data structure
struct ConfigData {
    uint8_t cfg_version = CONFIG_VERSION_CURRENT;
    
    /* Operating modes:
    * 0-dataMonitor: Basic data monitoring
    * 1-dataMonitorChart: Data monitoring with chart visualization
    * 2-dataMonitorCount: Data monitoring with counter
    */
    uint8_t defaultMode = 0;

    /* Default measurement channel selection:
    * 0-CHA (Channel A)
    * 1-CHB (Channel B)
    * (This value is used in dataMonitorChart and dataMonitorCount mode)
    */
    uint8_t defaultChannel = 0;

    /* Channel shunt resistor value in mΩ (milliohms)
    * 50mΩ provides:
    * - 0.8mA resolution with INA3221
    * - 3.6A maximum current measurement
    * Range: 0-255mΩ
    * (Usually you don't need to change this value, only if you use a different shunt resistor)
    */
    uint8_t shuntResistorCH1 = 50;
    uint8_t shuntResistorCH2 = 50;

    /* Serial communication control:
    * 0-disable: No serial output
    * 1-enable: Enable serial communication
    */
    uint8_t serialEnable = 1;

    /* Serial communication speed:
    * 0-115200 baud (currently the only supported rate)
    */
    uint8_t serialBaudrate = 0;

    /* Serial output format:
    * 0-human readable: Formatted text output
    * 1-arduino plotter: Compatible with Arduino Serial Plotter
    */
    uint8_t serialMode = 0;

    /* Serial data reporting interval:
    * 0-1000ms (1 second)
    * 1-500ms (0.5 second)
    * 2-100ms (0.1 second)
    * 3-50ms (0.05 second)
    * 4-10ms (0.01 second)
    */
    uint8_t serialPrintInterval = 0;

    /* Chart Y-axis scaling mode:
    * 0-fixed: Uses fixed scale defined by DEFAULT_CHART_SCALE
    * 1-auto: Automatically adjusts scale based on measurements
    */
    uint8_t chartUpdateInterval = 0;

    /* Fixed chart scale value (when DEFAULT_CHART_SCALE_MODE = 0)
    * Value represents hundreds of mA (e.g., 5 = 500mA)
    * Range: 1-255 (100mA-25.5A)
    */
    uint8_t chartScaleMode = 0;
    uint8_t chartScale = 5;
    
    // Validation methods
    bool isValid() const;
    void sanitize();
};

#endif