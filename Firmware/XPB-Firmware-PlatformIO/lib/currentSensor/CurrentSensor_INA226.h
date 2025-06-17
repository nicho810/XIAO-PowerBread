// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226


#ifndef CURRENT_SENSOR_INA226_H
#define CURRENT_SENSOR_INA226_H

#include <Wire.h>
#include "INA226.h"
#include "currentSensor.h"
#include "boardConfig.h" //Require the shunt resistor value and address
#include <vector>

// Single channel current sensor implementation
class cSensor_1ch_ina226 : public CurrentSensor {
public:
    // Constructor, initialize address, shunt resistor and averaging parameters
    cSensor_1ch_ina226(uint8_t address, float shuntResistorOhm, uint16_t average = 1);

    // Initialize sensor
    bool initialize() override;

    // Read sensor data
    bool readData(std::vector<currentSensorData>& data) override;

    // Get bus voltage (mV)
    float getBusVoltage_mV(uint8_t channel = 0) override;

    // Get shunt voltage (mV)
    float getShuntVoltage_mV(uint8_t channel = 0) override;

    // Get current (mA)
    float getCurrent_mA(uint8_t channel = 0) override;

    // Get power (mW)
    float getPower_mW(uint8_t channel = 0) override;

private:
    INA226 ina226_;  // INA226 sensor instance
};

// Dual channel current sensor implementation
class cSensor_2ch_ina226 : public CurrentSensor {
public:
    // Modified constructor to take two addresses
    cSensor_2ch_ina226(uint8_t address1, uint8_t address2, const std::vector<float>& shuntResistorOhms, uint16_t average = 1);

    // Initialize sensor
    bool initialize() override;

    // Read sensor data
    bool readData(std::vector<currentSensorData>& data) override;

    // Get bus voltage (mV)
    float getBusVoltage_mV(uint8_t channel = 0) override;

    // Get shunt voltage (mV)
    float getShuntVoltage_mV(uint8_t channel = 0) override;

    // Get current (mA)
    float getCurrent_mA(uint8_t channel = 0) override;

    // Get power (mW)
    float getPower_mW(uint8_t channel = 0) override;

private:
    std::vector<INA226> ina226s_;  // Two INA226 instances
    std::vector<uint8_t> addresses_;        // Addresses for each channel
};



#endif