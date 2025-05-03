#ifndef sysConfig_h
#define sysConfig_h

#include <Arduino.h>
#include <EEPROM.h>
#include "boardConfig.h"
#include "sysConfig_user.h"

struct sysConfig_addr
{
  uint8_t cfg_version = 0;
  uint8_t default_mode = 1;
  uint8_t default_channel = 2;
  uint8_t shuntResistorCHA = 3;
  uint8_t shuntResistorCHB = 4;
  uint8_t serial_enable = 5;
  uint8_t serial_baudRate = 6;
  uint8_t serial_mode = 7;
  uint8_t serial_printInterval = 8;
  uint8_t chart_updateInterval = 9;
  uint8_t chart_scaleMode = 10;
  uint8_t chart_scale = 11;
};

// Add an enum for indexing
enum ConfigIndex {
    CFG_DEFAULT_MODE = 0,
    CFG_DEFAULT_CHANNEL,
    CFG_SHUNT_RESISTOR_CHA,
    CFG_SHUNT_RESISTOR_CHB,
    CFG_SERIAL_ENABLE,
    CFG_SERIAL_BAUDRATE,
    CFG_SERIAL_MODE,
    CFG_SERIAL_PRINT_INTERVAL,
    CFG_CHART_UPDATE_INTERVAL,
    CFG_CHART_SCALE_MODE,
    CFG_CHART_SCALE,
    CFG_COUNT  // This will automatically hold the total number of configs
};

struct sysConfig_cfgName
{
    static const int MAX_NAME_LENGTH = 12;
    struct ConfigItem {
        char name[MAX_NAME_LENGTH];
    };
    
    ConfigItem items[CFG_COUNT] = {
        {"def_mode"},
        {"def_ch"},
        {"shunt_chA"},
        {"shunt_chB"},
        {"uart_en"},
        {"uart_baud"},
        {"uart_mode"},
        {"uart_intv"},
        {"chart_intv"},
        {"chart_M"},
        {"chart_SC"}
    };
};

struct sysConfig_data
{
  /*
  cfg_version tracks configuration structure changes
  Version history:
  - 0xFF (255): Default value in EEPROM
  - 0x00: Initial value after first firmware flash
  - 0x01: First version
  - 0x02: Added chart_scaleMode and chart_scale
  When a lower version is detected, default config will be written to EEPROM
  */
  uint8_t cfg_version = 2;

  // default mode: 0-dataMonitor, 1-dataMonitorChart, 2-dataMonitorCount
  uint8_t default_mode = DEFAULT_MODE;

  // default channel: 0-CHA, 1-CHB
  uint8_t default_channel = DEFAULT_CHANNEL;

  /*
  Shunt Resistor Configuration:
  - Uses 8-bit storage (0-255 mΩ range) since shunt resistors are typically below 100mΩ
  - Default: 50mΩ
    - With INA3221: 0.8mA resolution, 3.6A max current
  - Alternative: 20mΩ
    - With INA3221: 2mA resolution, 9A max current

  Note: Values are only written to EEPROM on first boot and persist across firmware updates.
  Can be modified anytime in config mode - see documentation:
  https://github.com/nicho810/XIAO-PowerBread/blob/main/Docs/sysConfig.md
  */
  uint8_t shuntResistorCHA = DEFAULT_SHUNT_CHA;
  uint8_t shuntResistorCHB = DEFAULT_SHUNT_CHB;

  // serial enable: 0-disable, 1-enable
  uint8_t serial_enable = DEFAULT_SERIAL_ENABLE;

  // serial baud rate: 0-115200, now only 115200 is supported
  uint8_t serial_baudRate = DEFAULT_SERIAL_BAUDRATE;

  // serial mode: 0-dafault mode (human readable message), 1-arduino plotter mode
  uint8_t serial_mode = DEFAULT_SERIAL_MODE;

  // serial print interval: 0-1000ms, 1-500ms, 2-100ms, 3-50ms, 4-10ms
  uint8_t serial_printInterval = DEFAULT_SERIAL_INTERVAL;

  // data chart interval: 0-50ms(default), 1-100ms, 2-250ms, 3-500ms, 4-1000ms
  uint8_t chart_updateInterval = DEFAULT_CHART_INTERVAL;

  // chart scale mode: 0-fixed(default), 1-auto
  uint8_t chart_scaleMode = DEFAULT_CHART_SCALE_MODE;

  // chart scale: 5=500mA(default), 0-255, 1 step is 100mA
  // only work when chart_scaleMode is 0
  uint8_t chart_scale = DEFAULT_CHART_SCALE;
};

class SysConfig
{
public:
  uint16_t cfg_size = 128; // each addr is 8bit(0-255), addr start from 0
  sysConfig_addr cfg_addr;
  sysConfig_data cfg_data;
  void begin_EEPROM();
  void init_EEPROM(int force_write = 0);
  void saveConfig_to_EEPROM(sysConfig_data data);
  void loadConfig_from_EEPROM();
  void loadConfig_from(sysConfig_data input_cfg_data);
  void incrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data);
  void decrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data);
  String output_all_config_data_in_String();
};

#endif