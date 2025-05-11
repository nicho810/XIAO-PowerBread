#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

struct currentSensorData {
  float busVoltage;
  float busCurrent;
  float busPower;
  float shuntVoltage;
  float shuntResistor;
  bool isDirty;
};

struct DualChannelData {
  currentSensorData channel0;
  currentSensorData channel1;
};


// Abstract base class for current sensors
class CurrentSensor {
public:
    virtual ~CurrentSensor() = default;
    virtual bool begin() = 0;
};


#endif
