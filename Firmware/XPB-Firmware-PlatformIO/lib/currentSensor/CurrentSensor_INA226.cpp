// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226

#include "CurrentSensor_INA226.h"

// Single channel current sensor implementation

cSensor_1ch_ina226::cSensor_1ch_ina226(uint8_t address, float shuntResistorOhm, float maxCurrent, uint16_t average)
    : CurrentSensor(address, {shuntResistorOhm}, {maxCurrent}, average), ina226_(address) {
    if (shuntResistorOhm <= 0) {
        Serial.println("Shunt resistor value must be positive");
    }
}

bool cSensor_1ch_ina226::initialize() {
    if (!ina226_.begin()) {
        Serial.print("Failed to initialize ina226 at address 0x");
        Serial.print(static_cast<int>(address_), HEX);
        Serial.println();
        return false;
    }
    // Set averaging parameters (ina226 library does not support direct setting of averaging count, here only print)
    Serial.print("> cSensor_1ch initialized at address 0x");
    Serial.print(static_cast<int>(address_), HEX);
    Serial.print(" with average: ");
    Serial.println(average_);
    return true;
}

bool cSensor_1ch_ina226::readData(std::vector<currentSensorData>& data) {
    data.clear();
    currentSensorData sensorData;
    sensorData.busVoltage_mV = ina226_.getBusVoltage_mV();
    sensorData.current_mA = ina226_.getCurrent_mA();
    sensorData.power_mW = ina226_.getPower_mW();
    sensorData.shuntVoltage_mV = ina226_.getShuntVoltage_mV();
    data.push_back(sensorData);
    return true;
}

float cSensor_1ch_ina226::getBusVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getBusVoltage_mV();
}

float cSensor_1ch_ina226::getShuntVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getShuntVoltage_mV();
}

float cSensor_1ch_ina226::getCurrent_mA(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getCurrent_mA();
}

float cSensor_1ch_ina226::getPower_mW(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getPower_mW();
}

// Dual channel current sensor implementation
cSensor_2ch_ina226::cSensor_2ch_ina226(uint8_t address1, uint8_t address2, const std::vector<float>& shuntResistorOhms, const std::vector<float>& maxCurrent, uint16_t average)
    : CurrentSensor(address1, shuntResistorOhms, maxCurrent, average) {
    if (shuntResistorOhms.size() != 2) {
        Serial.println("cSensor_2ch requires exactly 2 shunt resistor values");
        return;
    }
    for (float r : shuntResistorOhms) {
        if (r <= 0) {
            Serial.println("Shunt resistor values must be positive");
            return;
        }
    }
    if (maxCurrent.size() != 2) {
        Serial.println("cSensor_2ch requires exactly 2 max current values");
        return;
    }
    // Use the two provided addresses
    addresses_ = {address1, address2};
    ina226s_ = {INA226(address1), INA226(address2)};
    maxCurrent_ = maxCurrent;
}

bool cSensor_2ch_ina226::initialize() {
    for (size_t i = 0; i < ina226s_.size(); ++i) {
        if (!ina226s_[i].begin()) {
            Serial.print("Failed to initialize ina226 at address 0x");
            Serial.print(static_cast<int>(addresses_[i]), HEX);
            Serial.println();
            return false;
        }
    }

    //Set average
    ina226s_[0].setAverage(average_);
    ina226s_[1].setAverage(average_);

    //Set shunt resistor
    ina226s_[0].setMaxCurrentShunt(maxCurrent_[0], shuntResistorOhms_[0], true);
    ina226s_[1].setMaxCurrentShunt(maxCurrent_[1], shuntResistorOhms_[1], true);

    Serial.print("> cSensor #1 initialized: Address 0x");
    Serial.print(static_cast<int>(addresses_[0]), HEX);
    Serial.print(", Shunt Resistor ");
    Serial.print(ina226s_[0].getShunt(), 3);
    Serial.print(" Ohm, Max Current ");
    Serial.print(ina226s_[0].getMaxCurrent(), 3);
    Serial.print(" A");
    Serial.print(", Average ");
    Serial.print(average_);
    //LSB
    Serial.print(", LSB ");
    Serial.print(ina226s_[0].getCurrentLSB(), 3);
    Serial.print(" mA");
    Serial.println();

    Serial.print("> cSensor #2 initialized: Address 0x");
    Serial.print(static_cast<int>(addresses_[1]), HEX);
    Serial.print(", Shunt Resistor ");    
    Serial.print(ina226s_[1].getShunt(), 3);
    Serial.print(" Ohm, Max Current ");
    Serial.print(ina226s_[1].getMaxCurrent(), 3);
    Serial.print(" A");
    Serial.print(", Average ");
    Serial.print(average_);
    //LSB
    Serial.print(", LSB ");
    Serial.print(ina226s_[0].getCurrentLSB(), 3);
    Serial.print(" mA");
    Serial.println();


    return true;
}

bool cSensor_2ch_ina226::readData(std::vector<currentSensorData>& data) {
    data.clear();
    for (size_t i = 0; i < addresses_.size(); ++i) {
        currentSensorData sensorData;
        sensorData.busVoltage_mV = ina226s_[i].getBusVoltage_mV();
        sensorData.shuntVoltage_mV = ina226s_[i].getShuntVoltage_mV();
        sensorData.current_mA = ina226s_[i].getCurrent_mA();
        sensorData.power_mW = ina226s_[i].getPower_mW();
        data.push_back(sensorData);
    }
    return true;
}

float cSensor_2ch_ina226::getBusVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getBusVoltage_mV();
}

float cSensor_2ch_ina226::getShuntVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getShuntVoltage_mV();
}

float cSensor_2ch_ina226::getCurrent_mA(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getCurrent_mA();
}

float cSensor_2ch_ina226::getPower_mW(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getPower_mW();
}
