#include "sysConfig.h"

void sysConfig::saveConfig() {
    EEPROM.write(cfg_addr.default_mode, cfg_data.default_mode);
    EEPROM.write(cfg_addr.default_channel, cfg_data.default_channel);
    EEPROM.write(cfg_addr.shuntResistorCH1, cfg_data.shuntResistorCH1);
    EEPROM.write(cfg_addr.shuntResistorCH2, cfg_data.shuntResistorCH2);
    EEPROM.write(cfg_addr.serial_enable, cfg_data.serial_enable);
    EEPROM.write(cfg_addr.serial_baudRate, cfg_data.serial_baudRate);
    EEPROM.write(cfg_addr.serial_mode, cfg_data.serial_mode);
    EEPROM.write(cfg_addr.dataChart_interval, cfg_data.dataChart_interval);

    EEPROM.commit();
}

void sysConfig::loadConfig() {
    //load config from EEPROM
    cfg_data.default_mode = EEPROM.read(cfg_addr.default_mode);
    cfg_data.default_channel = EEPROM.read(cfg_addr.default_channel);
    cfg_data.shuntResistorCH1 = EEPROM.read(cfg_addr.shuntResistorCH1);
    cfg_data.shuntResistorCH2 = EEPROM.read(cfg_addr.shuntResistorCH2);
    cfg_data.serial_enable = EEPROM.read(cfg_addr.serial_enable);
    cfg_data.serial_baudRate = EEPROM.read(cfg_addr.serial_baudRate);
    cfg_data.serial_mode = EEPROM.read(cfg_addr.serial_mode);
    cfg_data.dataChart_interval = EEPROM.read(cfg_addr.dataChart_interval);
}


void sysConfig::begin() {
    EEPROM.begin(cfg_size);
}

void sysConfig::init() {
    if (EEPROM.read(cfg_addr.cfg_version) < cfg_data.cfg_version) {
        //detect a lower version, write default config to EEPROM
        //update cfg_version
        EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
        //write default config to EEPROM
        saveConfig();
    } else {
        //load config from EEPROM
        loadConfig();
    }
}

