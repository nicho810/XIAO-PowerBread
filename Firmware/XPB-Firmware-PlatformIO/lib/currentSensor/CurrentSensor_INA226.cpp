// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226

#include "CurrentSensor_INA226.h"

// Single channel current sensor implementation

cSensor_1ch::cSensor_1ch(uint8_t address, float shuntResistorOhm, uint16_t average)
    : CurrentSensor(address, {shuntResistorOhm}, average), ina226_(address) {
    if (shuntResistorOhm <= 0) {
        Serial.println("Shunt resistor value must be positive");
    }
}

bool cSensor_1ch::initialize() {
    if (!ina226_.begin()) {
        Serial.print("Failed to initialize ina226 at address 0x");
        Serial.print(static_cast<int>(address_), HEX);
        Serial.println();
        return false;
    }
    // Set averaging parameters (ina226 library does not support direct setting of averaging count, here only print)
    Serial.print("cSensor_1ch initialized at address 0x");
    Serial.print(static_cast<int>(address_), HEX);
    Serial.print(" with average: ");
    Serial.println(average_);
    return true;
}

bool cSensor_1ch::readData(std::vector<currentSensorData>& data) {
    data.clear();
    currentSensorData sensorData;
    sensorData.busVoltage_mV = ina226_.getBusVoltage_mV();
    sensorData.current_mA = ina226_.getCurrent_mA();
    sensorData.power_mW = ina226_.getPower_mW();
    sensorData.shuntVoltage_mV = ina226_.getShuntVoltage_mV();
    data.push_back(sensorData);
    return true;
}

float cSensor_1ch::getBusVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getBusVoltage_mV();
}

float cSensor_1ch::getShuntVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getShuntVoltage_mV();
}

float cSensor_1ch::getCurrent_mA(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getCurrent_mA();
}

float cSensor_1ch::getPower_mW(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getPower_mW();
}

// Dual channel current sensor implementation

cSensor_2ch::cSensor_2ch(uint8_t address1, uint8_t address2, const std::vector<float>& shuntResistorOhms, uint16_t average)
    : CurrentSensor(address1, shuntResistorOhms, average) {
    if (shuntResistorOhms.size() != 2) {
        Serial.println("cSensor_2ch requires exactly 2 shunt resistor values");
    }
    for (float r : shuntResistorOhms) {
        if (r <= 0) {
            Serial.println("Shunt resistor values must be positive");
        }
    }
    // Use the two provided addresses
    addresses_ = {address1, address2};
    ina226s_ = {INA226(address1), INA226(address2)};
}

bool cSensor_2ch::initialize() {
    for (size_t i = 0; i < ina226s_.size(); ++i) {
        if (!ina226s_[i].begin()) {
            Serial.print("Failed to initialize ina226 at address 0x");
            Serial.print(static_cast<int>(addresses_[i]), HEX);
            Serial.println();
            return false;
        }
    }
    Serial.print("cSensor_2ch initialized at base address 0x");
    Serial.print(static_cast<int>(address_), HEX);
    Serial.print(" with average: ");
    Serial.println(average_);
    return true;
}

bool cSensor_2ch::readData(std::vector<currentSensorData>& data) {
    data.clear();
    for (size_t i = 0; i < ina226s_.size(); ++i) {
        currentSensorData sensorData;
        sensorData.busVoltage_mV = ina226s_[i].getBusVoltage_mV();
        sensorData.shuntVoltage_mV = ina226s_[i].getShuntVoltage_mV();
        sensorData.current_mA = ina226s_[i].getCurrent_mA();
        sensorData.power_mW = ina226s_[i].getPower_mW();
        data.push_back(sensorData);
    }
    return true;
}

float cSensor_2ch::getBusVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getBusVoltage_mV();
}

float cSensor_2ch::getShuntVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getShuntVoltage_mV();
}

float cSensor_2ch::getCurrent_mA(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getCurrent_mA();
}

float cSensor_2ch::getPower_mW(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getPower_mW();
}
