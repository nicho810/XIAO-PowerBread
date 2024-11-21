#ifndef sysConfig_user_h
#define sysConfig_user_h

/* Default configuration values */

//Important Note: This default configuration value only write to EEPROM once when the first boot.
//After that, the configuration value will be read from EEPROM.

/* Operating modes:
 * 0-dataMonitor: Basic data monitoring
 * 1-dataMonitorChart: Data monitoring with chart visualization
 * 2-dataMonitorCount: Data monitoring with counter
 */
#define DEFAULT_MODE 0              

/* Default measurement channel selection:
 * 0-CHA (Channel A)
 * 1-CHB (Channel B)
 * (This value is used in dataMonitorChart and dataMonitorCount mode)
 */
#define DEFAULT_CHANNEL 0           

/* Channel A shunt resistor value in mΩ (milliohms)
 * 50mΩ provides:
 * - 0.8mA resolution with INA3221
 * - 3.6A maximum current measurement
 * Range: 0-255mΩ
 * (Usuallu you dont need to change this value, only if you use a different shunt resistor)
 */
#define DEFAULT_SHUNT_CHA 50        

/* Channel B shunt resistor value in mΩ (milliohms)
 * Same characteristics as Channel A
 */
#define DEFAULT_SHUNT_CHB 50        

/* Serial communication control:
 * 0-disable: No serial output
 * 1-enable: Enable serial communication
 */
#define DEFAULT_SERIAL_ENABLE 1     

/* Serial communication speed:
 * 0-115200 baud (currently the only supported rate)
 */
#define DEFAULT_SERIAL_BAUDRATE 0   

/* Serial output format:
 * 0-human readable: Formatted text output
 * 1-arduino plotter: Compatible with Arduino Serial Plotter
 */
#define DEFAULT_SERIAL_MODE 0       

/* Serial data reporting interval:
 * 0-1000ms (1 second)
 * 1-500ms (0.5 second)
 * 2-100ms (0.1 second)
 * 3-50ms (0.05 second)
 * 4-10ms (0.01 second)
 */
#define DEFAULT_SERIAL_INTERVAL 0   

/* Chart update/refresh interval:
 * 0-50ms (0.05 second, default)
 * 1-100ms (0.1 second)
 * 2-250ms (0.25 second)
 * 3-500ms (0.5 second)
 * 4-1000ms (1 second)
 */
#define DEFAULT_CHART_INTERVAL 0    

/* Chart Y-axis scaling mode:
 * 0-fixed: Uses fixed scale defined by DEFAULT_CHART_SCALE
 * 1-auto: Automatically adjusts scale based on measurements
 */
#define DEFAULT_CHART_SCALE_MODE 0  

/* Fixed chart scale value (when DEFAULT_CHART_SCALE_MODE = 0)
 * Value represents hundreds of mA (e.g., 5 = 500mA)
 * Range: 0-255 (0-25.5A)
 */
#define DEFAULT_CHART_SCALE 5       

#endif