#ifndef INA3221SENSOR_H
#define INA3221SENSOR_H

#include <Wire.h>
#include "INA3221.h"

struct INAData {
  float busVoltage;
  float current;
  float power;
  bool isDirty;
};

struct DualChannelData {
  INAData channel0;
  INAData channel1;
};

class INA3221Sensor {
public:
    INA3221Sensor(uint8_t address = 0x40);
    bool begin();
    void setShuntResistors(float shunt0, float shunt1);
    DualChannelData readCurrentSensors();

private:
    INA3221 ina;
    DualChannelData prevINAData;
};

#endif