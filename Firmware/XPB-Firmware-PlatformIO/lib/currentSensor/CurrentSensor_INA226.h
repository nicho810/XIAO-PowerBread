// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226


#ifndef CURRENT_SENSOR_INA226_H
#define CURRENT_SENSOR_INA226_H

#include <Wire.h>
#include "INA226.h"
#include "boardConfig.h"
#include "currentSensor.h"

// Make CurrentSensor_INA226 inherit from CurrentSensor
class CurrentSensor_INA226 : public CurrentSensor {
public:
    CurrentSensor_INA226(uint8_t address = 0x40);
    bool begin() override;
    void setShuntResistors(float shuntResistor);
    void setAverage(int average);
    DualChannelData readCurrentSensors();
    void setParameter(float shuntResistor = 0.050);
    INA226 ina226;
};

// Dual INA226
class CurrentSensor_DualINA226 {
public:
    CurrentSensor_DualINA226(uint8_t address1 = 0x40, uint8_t address2 = 0x41);
    bool begin();
    void setParameter(float shuntResistor1 = 0.050, float shuntResistor2 = 0.050);
    DualChannelData readCurrentSensors();

private:
    CurrentSensor_INA226 ina226_ch1;
    CurrentSensor_INA226 ina226_ch2; 
    DualChannelData prevINAData;
};

#endif