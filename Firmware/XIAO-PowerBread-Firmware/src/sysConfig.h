#ifndef sysConfig_h
#define sysConfig_h

#include <Arduino.h>
#include <EEPROM.h>

struct sysConfig_addr{
    uint8_t cfg_version=0;
    uint8_t default_mode=1;
    uint8_t default_channel=2;
    uint8_t shuntResistorCHA=3;
    uint8_t shuntResistorCHB=4;
    uint8_t serial_enable=5;
    uint8_t serial_baudRate=6;
    uint8_t serial_mode=7;
    uint8_t serial_printInterval=8;
    uint8_t chart_updateInterval=9;
    uint8_t chart_scaleMode=10;
    uint8_t chart_scale=11;
};

struct sysConfig_data{
    //cfg_version: increment when the config struct is changed, 
    //if detect a lower version, will write the default config to EEPROM
    //Usually the value of cfg_version is 0xFF(255), when first flash this firmware, it will be set to 0x00
    //Update log:
    //0x00 -> 0x01: first version
    //0x01 -> 0x02: add chart_scaleMode and chart_scale
    uint8_t cfg_version=2;

    //default mode: 0-dataMonitor, 1-dataMonitorChart, 2-dataMonitorCount
    uint8_t default_mode = 0; 

    //default channel: 0-CHA, 1-CHB
    uint8_t default_channel = 0; 

    //shunt resistor: usually the shunt resistor should below 100mOhm,
    //so we use 8bit to store the value, the max value is 255
    uint8_t shuntResistorCHA = 20;
    uint8_t shuntResistorCHB = 20;

    //serial enable: 0-disable, 1-enable
    uint8_t serial_enable = 1; 

    //serial baud rate: 0-115200, now only 115200 is supported
    uint8_t serial_baudRate=0; 

    //serial mode: 0-dafault mode (human readable message), 1-arduino plotter mode
    uint8_t serial_mode=0; 

    //serial print interval: 0-1000ms, 1-500ms, 2-100ms, 3-50ms, 4-10ms
    uint8_t serial_printInterval=0; 

    //data chart interval: 0-50ms(default), 1-100ms, 2-250ms, 3-500ms, 4-1000ms
    uint8_t chart_updateInterval=0; 

    //chart scale mode: 0-fixed(default), 1-auto
    uint8_t chart_scaleMode=0;

    //chart scale: 5=500mA(default), 0-255, 1 step is 100mA
    //only work when chart_scaleMode is 0
    uint8_t chart_scale=5;
};

class sysConfig {
    public:
        uint16_t cfg_size=128; //each addr is 8bit(0-255), addr start from 0
        sysConfig_addr cfg_addr;
        sysConfig_data cfg_data;
        void begin();
        void init(int force_write=0);
        void saveConfig(sysConfig_data data);
        void loadConfig();
        String debugPrintOnSerial();
};

void incrementConfigValue(int cursor, sysConfig_data& cfg_data);
void decrementConfigValue(int cursor, sysConfig_data& cfg_data);

#endif