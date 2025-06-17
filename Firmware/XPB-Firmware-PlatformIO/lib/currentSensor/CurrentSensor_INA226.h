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

// 单通道电流传感器实现
class cSensor_1ch : public CurrentSensor {
public:
    // 构造函数，初始化地址、分流电阻和平均参数
    cSensor_1ch(uint8_t address, float shuntResistorOhm, uint16_t average = 1);

    // 初始化传感器
    bool initialize() override;

    // 读取传感器数据
    bool readData(std::vector<currentSensorData>& data) override;

    // 获取总线电压 (mV)
    float getBusVoltage_mV(uint8_t channel = 0) override;

    // 获取分流电压 (mV)
    float getShuntVoltage_mV(uint8_t channel = 0) override;

    // 获取电流 (mA)
    float getCurrent_mA(uint8_t channel = 0) override;

    // 获取功率 (mW)
    float getPower_mW(uint8_t channel = 0) override;

private:
    INA226 ina226_;  // INA226 传感器实例
};

// 双通道电流传感器实现
class cSensor_2ch : public CurrentSensor {
public:
    // Modified constructor to take two addresses
    cSensor_2ch(uint8_t address1, uint8_t address2, const std::vector<float>& shuntResistorOhms, uint16_t average = 1);

    // 初始化传感器
    bool initialize() override;

    // 读取传感器数据
    bool readData(std::vector<currentSensorData>& data) override;

    // 获取总线电压 (mV)
    float getBusVoltage_mV(uint8_t channel = 0) override;

    // 获取分流电压 (mV)
    float getShuntVoltage_mV(uint8_t channel = 0) override;

    // 获取电流 (mA)
    float getCurrent_mA(uint8_t channel = 0) override;

    // 获取功率 (mW)
    float getPower_mW(uint8_t channel = 0) override;

private:
    std::vector<INA226> ina226s_;  // 两个 INA226 实例
    std::vector<uint8_t> addresses_;        // 各通道地址
};


// // Make CurrentSensor_INA226 inherit from CurrentSensor
// class CurrentSensor_INA226 : public CurrentSensor {
// public:
//     CurrentSensor_INA226(uint8_t address = currentSensor_0_address);
//     bool begin() override;
//     void setShuntResistors(float shuntResistor);
//     void setAverage(int average);
//     DualChannelData readCurrentSensors();
//     void setParameter(float shuntResistor = 0.050);
//     INA226 ina226;
// };

// // Dual INA226
// class CurrentSensor_DualINA226 {
// public:
//     CurrentSensor_DualINA226(uint8_t address1 = 0x40, uint8_t address2 = 0x41);
//     bool begin();
//     void setParameter(float shuntResistor1 = 0.050, float shuntResistor2 = 0.050);
//     DualChannelData readCurrentSensors();

// private:
//     CurrentSensor_INA226 ina226_ch1;
//     CurrentSensor_INA226 ina226_ch2; 
//     DualChannelData prevINAData;
// };

#endif