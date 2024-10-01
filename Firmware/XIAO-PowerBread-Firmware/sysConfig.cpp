#include <Arduino.h>
#include "sysConfig.h"


void sysConfig::saveConfig() {
    EEPROM.write(cfg_addr.default_mode, cfg_data.default_mode);
    EEPROM.write(cfg_addr.default_channel, cfg_data.default_channel);
    EEPROM.write(cfg_addr.shuntResistorCHA, cfg_data.shuntResistorCHA);
    EEPROM.write(cfg_addr.shuntResistorCHB, cfg_data.shuntResistorCHB);
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
    cfg_data.shuntResistorCHA = EEPROM.read(cfg_addr.shuntResistorCHA);
    cfg_data.shuntResistorCHB = EEPROM.read(cfg_addr.shuntResistorCHB);
    cfg_data.serial_enable = EEPROM.read(cfg_addr.serial_enable);
    cfg_data.serial_baudRate = EEPROM.read(cfg_addr.serial_baudRate);
    cfg_data.serial_mode = EEPROM.read(cfg_addr.serial_mode);
    cfg_data.dataChart_interval = EEPROM.read(cfg_addr.dataChart_interval);
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
        saveConfig();
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
            saveConfig();
        }
        else if (cfg_data.cfg_version < cfg_data.cfg_version) {
            //detect a lower version, write default config to EEPROM
            Serial.println("Detected a lower version, writing default config to EEPROM");
            //update cfg_version
            EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
            //write default config to EEPROM
            saveConfig();
        } else {
            //load config from EEPROM
            Serial.println("Loading config from EEPROM");
            loadConfig();
        }
    }
}

String sysConfig::debugPrintOnSerial() {
    String output = "Debug Print on Serial\n";
    output += "default_mode: " + String(cfg_data.default_mode) + "\n";
    output += "default_channel: " + String(cfg_data.default_channel) + "\n";
    output += "shuntResistorCHA: " + String(cfg_data.shuntResistorCHA) + "\n";
    output += "shuntResistorCHB: " + String(cfg_data.shuntResistorCHB) + "\n";
    output += "serial_enable: " + String(cfg_data.serial_enable) + "\n";
    output += "serial_baudRate: " + String(cfg_data.serial_baudRate) + "\n";
    output += "serial_mode: " + String(cfg_data.serial_mode) + "\n";
    output += "dataChart_interval: " + String(cfg_data.dataChart_interval) + "\n";
    return output;
}

