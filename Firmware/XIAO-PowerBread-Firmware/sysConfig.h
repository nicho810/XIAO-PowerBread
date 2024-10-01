#ifndef sysConfig_h
#define sysConfig_h

#include <EEPROM.h>

struct sysConfig_addr{
    uint8_t cfg_version=0; //increment when the config struct is changed
    uint8_t default_mode=1;
    uint8_t default_channel=2;
    uint8_t shuntResistorCH1=3;
    uint8_t shuntResistorCH2=4;
    uint8_t serial_enable=5;
    uint8_t serial_baudRate=6;
    uint8_t serial_mode=7;
    uint8_t dataChart_interval=8;
};

struct sysConfig_data{
    //cfg_version: increment when the config struct is changed, 
    //if detect a lower version, will write the default config to EEPROM
    uint8_t cfg_version=1;
    //default mode: 0-dataMonitor, 1-dataMonitorChart, 2-dataMonitorCount
    uint8_t default_mode = 0; 
    //default channel: 0-CH1, 1-CH2
    uint8_t default_channel = 0; 
    //shunt resistor: 0-20mOhm, 1-50mOhm, 2-100mOhm, 3-200mOhm
    uint8_t shuntResistorCH1 = 20;
    uint8_t shuntResistorCH2 = 20;
    //serial enable: 0-disable, 1-enable
    uint8_t serial_enable = 1; 
    //serial baud rate: 0-9600, 1-19200, 2-38400, 3-57600, 4-115200
    uint8_t serial_baudRate=1; 
    //serial mode: 0-dafault mode
    uint8_t serial_mode=0; 
    //data chart interval: default
    uint8_t dataChart_interval=0; 
};

class sysConfig {
    public:
        uint16_t cfg_size=128; //each addr is 8bit(0-255), addr start from 0
        sysConfig_addr cfg_addr;
        sysConfig_data cfg_data;
        void begin();
        void init();
        void saveConfig();
        void loadConfig();
};

#endif