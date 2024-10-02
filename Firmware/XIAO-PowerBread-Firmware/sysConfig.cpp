#include <Arduino.h>
#include "sysConfig.h"


void sysConfig::saveConfig(sysConfig_data data) {
    EEPROM.write(cfg_addr.default_mode, data.default_mode);
    EEPROM.write(cfg_addr.default_channel, data.default_channel);
    EEPROM.write(cfg_addr.shuntResistorCHA, data.shuntResistorCHA);
    EEPROM.write(cfg_addr.shuntResistorCHB, data.shuntResistorCHB);
    EEPROM.write(cfg_addr.serial_enable, data.serial_enable);
    EEPROM.write(cfg_addr.serial_baudRate, data.serial_baudRate);
    EEPROM.write(cfg_addr.serial_mode, data.serial_mode);
    EEPROM.write(cfg_addr.serial_printInterval, data.serial_printInterval);
    EEPROM.write(cfg_addr.chart_updateInterval, data.chart_updateInterval);

    EEPROM.commit();
}

void sysConfig::loadConfig() {
    //load config from EEPROM
    cfg_data.cfg_version = EEPROM.read(cfg_addr.cfg_version);
    cfg_data.default_mode = EEPROM.read(cfg_addr.default_mode);
    cfg_data.default_channel = EEPROM.read(cfg_addr.default_channel);
    cfg_data.shuntResistorCHA = EEPROM.read(cfg_addr.shuntResistorCHA);
    cfg_data.shuntResistorCHB = EEPROM.read(cfg_addr.shuntResistorCHB);
    cfg_data.serial_enable = EEPROM.read(cfg_addr.serial_enable);
    cfg_data.serial_baudRate = EEPROM.read(cfg_addr.serial_baudRate);
    cfg_data.serial_mode = EEPROM.read(cfg_addr.serial_mode);
    cfg_data.serial_printInterval = EEPROM.read(cfg_addr.serial_printInterval);
    cfg_data.chart_updateInterval = EEPROM.read(cfg_addr.chart_updateInterval);
}


void sysConfig::begin() {
    EEPROM.begin(cfg_size);
}

void sysConfig::init(int force_write) {
    //if force_write is true, will write default config to EEPROM
    if (force_write) {
        Serial.println("Force write default config to EEPROM");
        //update cfg_version
        EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
        //write default config to EEPROM
        saveConfig(cfg_data);
    }
    else{
        //reed cfg_version
        cfg_data.cfg_version = EEPROM.read(cfg_addr.cfg_version);
        //if cfg_version==0xFF, it means EEPROM is not initialized
        if (cfg_data.cfg_version == 0xFF) {
            Serial.println("EEPROM is not initialized, writing default config to EEPROM");
            //update cfg_version
            EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
            //write default config to EEPROM
            saveConfig(cfg_data);
        }
        else if (cfg_data.cfg_version < cfg_data.cfg_version) {
            //detect a lower version, write default config to EEPROM
            Serial.println("Detected a lower version, writing default config to EEPROM");
            //update cfg_version
            EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
            //write default config to EEPROM
            saveConfig(cfg_data);
        } else {
            //load config from EEPROM
            Serial.println("Loading config from EEPROM");
            loadConfig();
        }
    }
}

String sysConfig::debugPrintOnSerial() {
    String output = "\n";
    output += "cfg_version: " + String(cfg_data.cfg_version) + "\n";
    output += "default_mode: " + String(cfg_data.default_mode) + "\n";
    output += "default_channel: " + String(cfg_data.default_channel) + "\n";
    output += "shuntResistorCHA: " + String(cfg_data.shuntResistorCHA) + "\n";
    output += "shuntResistorCHB: " + String(cfg_data.shuntResistorCHB) + "\n";
    output += "serial_enable: " + String(cfg_data.serial_enable) + "\n";
    output += "serial_baudRate: " + String(cfg_data.serial_baudRate) + "\n";
    output += "serial_mode: " + String(cfg_data.serial_mode) + "\n";
    output += "serial_printInterval: " + String(cfg_data.serial_printInterval) + "\n";
    output += "chart_updateInterval: " + String(cfg_data.chart_updateInterval) + "\n";
    return output;
}


//Adjust the range of the config value here
void incrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data) {
    switch (cursor) {
        case 0: tmp_cfg_data.default_mode = min(tmp_cfg_data.default_mode + 1, 2); break;
        case 1: tmp_cfg_data.default_channel = min(tmp_cfg_data.default_channel + 1, 1); break;
        case 2: tmp_cfg_data.shuntResistorCHA = min(tmp_cfg_data.shuntResistorCHA + 1, 255); break;
        case 3: tmp_cfg_data.shuntResistorCHB = min(tmp_cfg_data.shuntResistorCHB + 1, 255); break;
        case 4: tmp_cfg_data.serial_enable = !tmp_cfg_data.serial_enable; break;
        case 5: tmp_cfg_data.serial_baudRate = min(tmp_cfg_data.serial_baudRate + 1, 0); break;
        case 6: tmp_cfg_data.serial_mode = min(tmp_cfg_data.serial_mode + 1, 1); break;
        case 7: tmp_cfg_data.serial_printInterval = min(tmp_cfg_data.serial_printInterval + 1, 4); break;
        case 8: tmp_cfg_data.chart_updateInterval = min(tmp_cfg_data.chart_updateInterval + 1, 0); break;
    }
}

void decrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data) {
    switch (cursor) {
        case 0: tmp_cfg_data.default_mode = max(tmp_cfg_data.default_mode - 1, 0); break;
        case 1: tmp_cfg_data.default_channel = max(tmp_cfg_data.default_channel - 1, 0); break;
        case 2: tmp_cfg_data.shuntResistorCHA = max(tmp_cfg_data.shuntResistorCHA - 1, 0); break;
        case 3: tmp_cfg_data.shuntResistorCHB = max(tmp_cfg_data.shuntResistorCHB - 1, 0); break;
        case 4: tmp_cfg_data.serial_enable = !tmp_cfg_data.serial_enable; break;
        case 5: tmp_cfg_data.serial_baudRate = max(tmp_cfg_data.serial_baudRate - 1, 0); break;
        case 6: tmp_cfg_data.serial_mode = max(tmp_cfg_data.serial_mode - 1, 0); break;
        case 7: tmp_cfg_data.serial_printInterval = max(tmp_cfg_data.serial_printInterval - 1, 0); break;
        case 8: tmp_cfg_data.chart_updateInterval = max(tmp_cfg_data.chart_updateInterval - 1, 0); break;
    }
}

