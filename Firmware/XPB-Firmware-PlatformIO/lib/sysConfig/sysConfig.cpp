#include <Arduino.h>
#include "sysConfig.h"

// ConfigData validation and sanitization
bool ConfigData::isValid() const {
    return (defaultMode <= MAX_MODE &&
            defaultChannel <= MAX_CHANNEL &&
            shuntResistorCH1 <= MAX_SHUNT_RESISTOR &&
            shuntResistorCH2 <= MAX_SHUNT_RESISTOR &&
            serialBaudrate <= MAX_SERIAL_BAUDRATE &&
            serialMode <= MAX_SERIAL_MODE &&
            serialPrintInterval <= MAX_SERIAL_INTERVAL &&
            chartUpdateInterval <= MAX_CHART_INTERVAL &&
            chartScaleMode <= MAX_CHART_SCALE_MODE &&
            chartScale >= MIN_CHART_SCALE &&
            chartScale <= MAX_CHART_SCALE);
}

void ConfigData::sanitize() {
    defaultMode = min(defaultMode, MAX_MODE);
    defaultChannel = min(defaultChannel, MAX_CHANNEL);
    shuntResistorCH1 = min(shuntResistorCH1, MAX_SHUNT_RESISTOR);
    shuntResistorCH2 = min(shuntResistorCH2, MAX_SHUNT_RESISTOR);
    serialBaudrate = min(serialBaudrate, MAX_SERIAL_BAUDRATE);
    serialMode = min(serialMode, MAX_SERIAL_MODE);
    serialPrintInterval = min(serialPrintInterval, MAX_SERIAL_INTERVAL);
    chartUpdateInterval = min(chartUpdateInterval, MAX_CHART_INTERVAL);
    chartScaleMode = min(chartScaleMode, MAX_CHART_SCALE_MODE);
    chartScale = constrain(chartScale, MIN_CHART_SCALE, MAX_CHART_SCALE);
}

// SysConfig implementation
SysConfig::SysConfig() {
}

bool SysConfig::begin() {
    #if defined(Proj_XIAOPowerBread)
    EEPROM.begin(EEPROM_SIZE);
    #elif defined(Proj_XIAOPowerMonitor)
    extEEPROM.setMemoryType(2); //24C02
    extEEPROM.begin(0x50, Wire);
    #endif
    eepromInitialized_ = true;
    return true;
}

bool SysConfig::initialize(bool forceWrite) {
    if (!eepromInitialized_) {
        begin();
    }
    
    if (forceWrite) {
        Serial.println("Force writing default config to EEPROM");
        return resetToDefaults();
    }
    
    uint8_t currentVersion = readFromEEPROM(ConfigAddresses::CFG_VERSION);
    Serial.print("Current version: ");
    Serial.println(currentVersion);
    
    if (currentVersion == CONFIG_VERSION_UNINITIALIZED) {
        Serial.println("EEPROM not initialized, writing default config");
        return resetToDefaults();
    } else if (currentVersion < CONFIG_VERSION_CURRENT) {
        Serial.println("Detected older config version, updating to new versions");
        return resetToDefaults();
    } else {
        Serial.println("Loading config from EEPROM");
        return loadFromEEPROM();
    }
}

bool SysConfig::setConfig(const ConfigData& newConfig) {
    if (!newConfig.isValid()) {
        return false;
    }
    
    cfgData_ = newConfig;
    return saveToEEPROM();
}

bool SysConfig::incrementValue(ConfigIndex index) {
    uint8_t currentValue = getConfigValue(index);
    uint8_t newValue = currentValue;
    
    if (isToggleType(index)) {
        newValue = !currentValue;
    } else {
        newValue = min((int)currentValue + 1, (int)getMaxValue(index));
    }
    
    return setConfigValue(index, newValue);
}

bool SysConfig::decrementValue(ConfigIndex index) {
    uint8_t currentValue = getConfigValue(index);
    uint8_t newValue = currentValue;
    
    if (isToggleType(index)) {
        newValue = !currentValue;
    } else {
        newValue = max((int)currentValue - 1, (int)getMinValue(index));
    }
    
    return setConfigValue(index, newValue);
}

bool SysConfig::saveToEEPROM() {
    if (!eepromInitialized_) {
        return false;
    }

    //Write current config data to EEPROM
    writeToEEPROM(ConfigAddresses::CFG_VERSION, cfgData_.cfg_version);
    writeToEEPROM(ConfigAddresses::DEFAULT_MODE, cfgData_.defaultMode);
    writeToEEPROM(ConfigAddresses::DEFAULT_CHANNEL, cfgData_.defaultChannel);
    writeToEEPROM(ConfigAddresses::SHUNT_RESISTOR_CHA, cfgData_.shuntResistorCH1);
    writeToEEPROM(ConfigAddresses::SHUNT_RESISTOR_CHB, cfgData_.shuntResistorCH2);
    writeToEEPROM(ConfigAddresses::SERIAL_ENABLE, cfgData_.serialEnable);
    writeToEEPROM(ConfigAddresses::SERIAL_BAUDRATE, cfgData_.serialBaudrate);
    writeToEEPROM(ConfigAddresses::SERIAL_MODE, cfgData_.serialMode);
    writeToEEPROM(ConfigAddresses::SERIAL_PRINT_INTERVAL, cfgData_.serialPrintInterval);
    writeToEEPROM(ConfigAddresses::CHART_UPDATE_INTERVAL, cfgData_.chartUpdateInterval);
    writeToEEPROM(ConfigAddresses::CHART_SCALE_MODE, cfgData_.chartScaleMode);
    writeToEEPROM(ConfigAddresses::CHART_SCALE, cfgData_.chartScale);

    #if defined(Proj_XIAOPowerBread)
    EEPROM.commit();
    #elif defined(Proj_XIAOPowerMonitor)
    //no need to commit for external EEPROM
    #endif

    return true;
}

bool SysConfig::loadFromEEPROM() {
    if (!eepromInitialized_) {
        return false;
    } 
    cfgData_.cfg_version = readFromEEPROM(ConfigAddresses::CFG_VERSION);
    cfgData_.defaultMode = readFromEEPROM(ConfigAddresses::DEFAULT_MODE);
    cfgData_.defaultChannel = readFromEEPROM(ConfigAddresses::DEFAULT_CHANNEL);
    cfgData_.shuntResistorCH1 = readFromEEPROM(ConfigAddresses::SHUNT_RESISTOR_CHA);
    cfgData_.shuntResistorCH2 = readFromEEPROM(ConfigAddresses::SHUNT_RESISTOR_CHB);
    cfgData_.serialEnable = readFromEEPROM(ConfigAddresses::SERIAL_ENABLE);
    cfgData_.serialBaudrate = readFromEEPROM(ConfigAddresses::SERIAL_BAUDRATE);
    cfgData_.serialMode = readFromEEPROM(ConfigAddresses::SERIAL_MODE);
    cfgData_.serialPrintInterval = readFromEEPROM(ConfigAddresses::SERIAL_PRINT_INTERVAL);
    cfgData_.chartUpdateInterval = readFromEEPROM(ConfigAddresses::CHART_UPDATE_INTERVAL);
    cfgData_.chartScaleMode = readFromEEPROM(ConfigAddresses::CHART_SCALE_MODE);
    cfgData_.chartScale = readFromEEPROM(ConfigAddresses::CHART_SCALE);
    cfgData_.sanitize();
 
    return cfgData_.isValid();
}

bool SysConfig::resetToDefaults() {
    cfgData_ = ConfigData(); // Reset to defaults
    return saveToEEPROM();
}

String SysConfig::toString() const {
    String output = "\n----------[Config Data]----------\n";
    output += "Version: " + String(cfgData_.cfg_version) + "\n";
    output += "Default Mode: " + String(cfgData_.defaultMode) + "\n";
    output += "Default Channel: " + String(cfgData_.defaultChannel) + "\n";
    output += "Shunt Resistor CHA: " + String(cfgData_.shuntResistorCH1) + " mΩ\n";
    output += "Shunt Resistor CHB: " + String(cfgData_.shuntResistorCH2) + " mΩ\n";
    output += "Serial Enable: " + String(cfgData_.serialEnable ? "Yes" : "No") + "\n";
    output += "Serial Baudrate: " + String(cfgData_.serialBaudrate) + "\n";
    output += "Serial Mode: " + String(cfgData_.serialMode) + "\n";
    output += "Serial Print Interval: " + String(cfgData_.serialPrintInterval) + "\n";
    output += "Chart Update Interval: " + String(cfgData_.chartUpdateInterval) + "\n";
    output += "Chart Scale Mode: " + String(cfgData_.chartScaleMode) + "\n";
    output += "Chart Scale: " + String(cfgData_.chartScale) + "\n";
    output += "--------[Config Data End]--------\n";
    return output;
}

// Static helper methods
uint8_t SysConfig::getMinValue(ConfigIndex index) {
    switch (index) {
        case ConfigIndex::CHART_SCALE: return MIN_CHART_SCALE;
        default: return 0;
    }
}

uint8_t SysConfig::getMaxValue(ConfigIndex index) {
    switch (index) {
        case ConfigIndex::DEFAULT_MODE: return MAX_MODE;
        case ConfigIndex::DEFAULT_CHANNEL: return MAX_CHANNEL;
        case ConfigIndex::SHUNT_RESISTOR_CHA:
        case ConfigIndex::SHUNT_RESISTOR_CHB: return MAX_SHUNT_RESISTOR;
        case ConfigIndex::SERIAL_BAUDRATE: return MAX_SERIAL_BAUDRATE;
        case ConfigIndex::SERIAL_MODE: return MAX_SERIAL_MODE;
        case ConfigIndex::SERIAL_PRINT_INTERVAL: return MAX_SERIAL_INTERVAL;
        case ConfigIndex::CHART_UPDATE_INTERVAL: return MAX_CHART_INTERVAL;
        case ConfigIndex::CHART_SCALE_MODE: return MAX_CHART_SCALE_MODE;
        case ConfigIndex::CHART_SCALE: return MAX_CHART_SCALE;
        default: return 1; // For boolean values
    }
}

bool SysConfig::isToggleType(ConfigIndex index) {
    return index == ConfigIndex::SERIAL_ENABLE;
}

// Private helper methods
bool SysConfig::writeToEEPROM(uint8_t address, uint8_t value) {
    if (!eepromInitialized_) {
        return false;
    }
    #if defined(Proj_XIAOPowerBread)
    EEPROM.write(address, value);
    #elif defined(Proj_XIAOPowerMonitor)
    extEEPROM.put(address, value);
    #endif
    return true;
}

uint8_t SysConfig::readFromEEPROM(uint8_t address) {
    #if defined(Proj_XIAOPowerBread)
    if (!eepromInitialized_) {
        return 0;
    }
    return EEPROM.read(address);
    #elif defined(Proj_XIAOPowerMonitor)
    uint8_t value;
    extEEPROM.get(address, value);
    return value;
    #endif
}


// Helper methods for getting/setting individual config values
uint8_t SysConfig::getConfigValue(ConfigIndex index) {
    switch (index) {
        case ConfigIndex::DEFAULT_MODE: return cfgData_.defaultMode;
        case ConfigIndex::DEFAULT_CHANNEL: return cfgData_.defaultChannel;
        case ConfigIndex::SHUNT_RESISTOR_CHA: return cfgData_.shuntResistorCH1;
        case ConfigIndex::SHUNT_RESISTOR_CHB: return cfgData_.shuntResistorCH2;
        case ConfigIndex::SERIAL_ENABLE: return cfgData_.serialEnable;
        case ConfigIndex::SERIAL_BAUDRATE: return cfgData_.serialBaudrate;
        case ConfigIndex::SERIAL_MODE: return cfgData_.serialMode;
        case ConfigIndex::SERIAL_PRINT_INTERVAL: return cfgData_.serialPrintInterval;
        case ConfigIndex::CHART_UPDATE_INTERVAL: return cfgData_.chartUpdateInterval;
        case ConfigIndex::CHART_SCALE_MODE: return cfgData_.chartScaleMode;
        case ConfigIndex::CHART_SCALE: return cfgData_.chartScale;
        default: return 0;
    }
}

bool SysConfig::setConfigValue(ConfigIndex index, uint8_t value) {
    if (!validateConfigValue(index, value)) {
        return false;
    }
    
    switch (index) {
        case ConfigIndex::DEFAULT_MODE: cfgData_.defaultMode = value; break;
        case ConfigIndex::DEFAULT_CHANNEL: cfgData_.defaultChannel = value; break;
        case ConfigIndex::SHUNT_RESISTOR_CHA: cfgData_.shuntResistorCH1 = value; break;
        case ConfigIndex::SHUNT_RESISTOR_CHB: cfgData_.shuntResistorCH2 = value; break;
        case ConfigIndex::SERIAL_ENABLE: cfgData_.serialEnable = value; break;
        case ConfigIndex::SERIAL_BAUDRATE: cfgData_.serialBaudrate = value; break;
        case ConfigIndex::SERIAL_MODE: cfgData_.serialMode = value; break;
        case ConfigIndex::SERIAL_PRINT_INTERVAL: cfgData_.serialPrintInterval = value; break;
        case ConfigIndex::CHART_UPDATE_INTERVAL: cfgData_.chartUpdateInterval = value; break;
        case ConfigIndex::CHART_SCALE_MODE: cfgData_.chartScaleMode = value; break;
        case ConfigIndex::CHART_SCALE: cfgData_.chartScale = value; break;
        default: return false;
    }
    
    return true;
}

bool SysConfig::validateConfigValue(ConfigIndex index, uint8_t value) {
    if (isToggleType(index)) {
        return value <= 1;
    }
    
    uint8_t minVal = getMinValue(index);
    uint8_t maxVal = getMaxValue(index);
    return value >= minVal && value <= maxVal;
}


