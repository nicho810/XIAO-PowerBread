// This class is based on the INA3221 library by Rob Tillaart
// Author: Rob Tillaart
// Purpose: demo polling 3 channels
// URL: https://github.com/RobTillaart/INA3221_RT


#ifndef CURRENT_SENSOR_INA3221_H
#define CURRENT_SENSOR_INA3221_H

#include <Wire.h>
#include "INA3221.h"
#include "boardConfig.h"
#include "currentSensor.h"

// Make CurrentSensor_INA3221 inherit from CurrentSensor
class CurrentSensor_INA3221 : public CurrentSensor {
public:
    CurrentSensor_INA3221(uint8_t address = 0x40);
    bool begin() override;
    void setShuntResistors(float shunt0, float shunt1);
    void disableChannel(int channel);
    void setAverage(int average) ;
    DualChannelData readCurrentSensors() ;
    void setParameter(float shuntResistorCHA = 0.050, float shuntResistorCHB = 0.050) ;

private:
    INA3221 ina;
    DualChannelData prevINAData;
};



#endif