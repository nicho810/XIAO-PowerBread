/**
 * [INPUT]: Wire.h (I2C), INA3221_RT 库, boardConfig.h (引脚)
 * [OUTPUT]: INAData / DualChannelData 结构体, INA3221Sensor 类 (begin/read/setParameter)
 * [POS]: 传感器抽象层，双通道电压/电流/功率采集的单一真相源，isDirty 驱动选择性 UI 刷新
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 *
 * 基于 Rob Tillaart 的 INA3221_RT 库: https://github.com/RobTillaart/INA3221_RT
 */

#ifndef INA3221SENSOR_H
#define INA3221SENSOR_H

#include <Wire.h>
#include "INA3221.h"
#include "boardConfig.h"

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
    INA3221Sensor(uint8_t address = 0x40);  // Constructor only stores address
    bool begin();
    void setShuntResistors(float shunt0, float shunt1);
    void disableChannel(int channel);
    void setAverage(int average);
    DualChannelData readCurrentSensors();
    void setParameter(float shuntResistorCHA = 0.050, float shuntResistorCHB = 0.050);

private:
    INA3221 ina;
    DualChannelData prevINAData;
};

#endif