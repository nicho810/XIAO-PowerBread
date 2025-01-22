#include <Arduino.h>
#include "sysConfig.h"

SemaphoreHandle_t configStateMutex = NULL;


void SysConfig::saveConfig_to_EEPROM(sysConfig_data data) {
    EEPROM.write(cfg_addr.default_mode, data.default_mode);
    EEPROM.write(cfg_addr.default_channel, data.default_channel);
    EEPROM.write(cfg_addr.shuntResistorCHA, data.shuntResistorCHA);
    EEPROM.write(cfg_addr.shuntResistorCHB, data.shuntResistorCHB);
    EEPROM.write(cfg_addr.serial_enable, data.serial_enable);
    EEPROM.write(cfg_addr.serial_baudRate, data.serial_baudRate);
    EEPROM.write(cfg_addr.serial_mode, data.serial_mode);
    EEPROM.write(cfg_addr.serial_printInterval, data.serial_printInterval);
    EEPROM.write(cfg_addr.chart_updateInterval, data.chart_updateInterval);
    EEPROM.write(cfg_addr.chart_scaleMode, data.chart_scaleMode); //start from 0x02
    EEPROM.write(cfg_addr.chart_scale, data.chart_scale); //start from 0x02

    EEPROM.commit();
}

void SysConfig::loadConfig_from(sysConfig_data input_cfg_data) {
    cfg_data = input_cfg_data;
}

void SysConfig::loadConfig_from_EEPROM() {
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
    cfg_data.chart_scaleMode = EEPROM.read(cfg_addr.chart_scaleMode); //start from 0x02
    cfg_data.chart_scale = EEPROM.read(cfg_addr.chart_scale); //start from 0x02
}


void SysConfig::begin_EEPROM() {
    EEPROM.begin(cfg_size);
}

void SysConfig::init_EEPROM(int force_write) {
    //if force_write is true, will write default config to EEPROM
    if (force_write) {
        Serial.println("Force write default config to EEPROM");
        //update cfg_version
        EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
        //write default config to EEPROM
        saveConfig_to_EEPROM(cfg_data);
    }
    else{
        //read cfg_version
        uint8_t current_cfg_version = EEPROM.read(cfg_addr.cfg_version);
        //if cfg_version==0xFF, it means EEPROM is not initialized
        if (current_cfg_version == 0xFF) {
            Serial.println("EEPROM is not initialized, writing default config to EEPROM");
            //update cfg_version
            EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
            //write default config to EEPROM
            saveConfig_to_EEPROM(cfg_data);
        }
        else if (current_cfg_version < cfg_data.cfg_version) {
            //detect a lower version, write default config to EEPROM
            Serial.println("Detected a lower version, writing default config to EEPROM");
            //update cfg_version
            EEPROM.write(cfg_addr.cfg_version, cfg_data.cfg_version);
            //write default config to EEPROM
            saveConfig_to_EEPROM(cfg_data);
        } else {
            //load config from EEPROM
            Serial.println("Loading config from EEPROM");
            loadConfig_from_EEPROM();
        }
    }
}

String SysConfig::output_all_config_data_in_String() {
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
    output += "chart_scaleMode: " + String(cfg_data.chart_scaleMode) + "\n";
    output += "chart_scale: " + String(cfg_data.chart_scale) + "\n";
    return output;
}


//Adjust the range of the config value here
void SysConfig::incrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data) {
    switch (cursor) {
        case 0: tmp_cfg_data.default_mode = min(tmp_cfg_data.default_mode + 1, 2); break;
        case 1: tmp_cfg_data.default_channel = min(tmp_cfg_data.default_channel + 1, 1); break;
        case 2: tmp_cfg_data.shuntResistorCHA = min(tmp_cfg_data.shuntResistorCHA + 1, 255); break;
        case 3: tmp_cfg_data.shuntResistorCHB = min(tmp_cfg_data.shuntResistorCHB + 1, 255); break;
        case 4: tmp_cfg_data.serial_enable = !tmp_cfg_data.serial_enable; break;
        case 5: tmp_cfg_data.serial_baudRate = min(tmp_cfg_data.serial_baudRate + 1, 0); break;
        case 6: tmp_cfg_data.serial_mode = min(tmp_cfg_data.serial_mode + 1, 1); break;
        case 7: tmp_cfg_data.serial_printInterval = min(tmp_cfg_data.serial_printInterval + 1, 4); break;
        case 8: tmp_cfg_data.chart_updateInterval = min(tmp_cfg_data.chart_updateInterval + 1, 4); break;
        case 9: tmp_cfg_data.chart_scaleMode = min(tmp_cfg_data.chart_scaleMode + 1, 1); break;
        case 10: tmp_cfg_data.chart_scale = min(tmp_cfg_data.chart_scale + 1, 255); break;
    }
}

void SysConfig::decrementConfigValue(int cursor, sysConfig_data& tmp_cfg_data) {
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
        case 9: tmp_cfg_data.chart_scaleMode = max(tmp_cfg_data.chart_scaleMode - 1, 0); break;
        case 10: tmp_cfg_data.chart_scale = max(tmp_cfg_data.chart_scale - 1, 1); break; //can't be 0, min is 1
    }
}



// Helper functions to safely modify state
bool ConfigMode::enterConfigMode() {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = true;
        xSemaphoreGive(configStateMutex);
        return !wasActive; // Return true if state changed
    }
    return false;
}

bool ConfigMode::exitConfigMode() {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = false;
        configState.cursor = 0;
        configState.cursorLast = 0;
        configState.cursorStatus = 0;
        xSemaphoreGive(configStateMutex);
        return wasActive; // Return true if state changed
    }
    return false;
}

void ConfigMode::updateConfigCursor(int8_t newCursor) {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        if (newCursor >= 0 && newCursor < configState.cursorMax) {
            configState.cursorLast = configState.cursor;
            configState.cursor = newCursor;
        }
        xSemaphoreGive(configStateMutex);
    }
}

bool ConfigMode::getConfigState(ConfigModeState *state)
{
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        memcpy(state, &configState, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}

bool ConfigMode::updateConfigState(const ConfigModeState *state)
{
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        memcpy(&configState, state, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}