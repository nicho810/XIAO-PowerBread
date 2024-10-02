#ifndef INA3221SENSOR_H
#define INA3221SENSOR_H

#include <Wire.h>
#include "INA3221.h"

struct INAData {
  float busVoltage;
  float busCurrent;
  float busPower;
  float shuntVoltage;
  float shuntResistor;
  bool isDirty;
};

struct DualChannelData {
  INAData channel0;
  INAData channel1;
};

class INA3221Sensor {
public:
    INA3221Sensor(uint8_t address = 0x40);
    bool begin(float shuntResistorCHA, float shuntResistorCHB);
    void setShuntResistors(float shunt0, float shunt1);
    void disableChannel(int channel);
    void setAverage(int average);
    DualChannelData readCurrentSensors();

private:
    INA3221 ina;
    DualChannelData prevINAData;
};

#endif