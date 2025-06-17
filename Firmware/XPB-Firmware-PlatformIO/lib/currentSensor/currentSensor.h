#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include <cstdint>
#include <vector>

struct currentSensorData {
  float busVoltage_mV;
  float shuntVoltage_mV;
  float current_mA;
  float power_mW;
  // float shuntResistor_mOhm;
};

// struct Dual_currentSensorData {
//   currentSensorData channel0;
//   currentSensorData channel1;
// };


// // Abstract base class for current sensors
// //Para: address, shuntResistor_mOhm, , average
// class CurrentSensor {
// public:
//     virtual ~CurrentSensor() = default;
//     virtual bool begin() = 0;
//     virtual void setParameter(uint8_t address, float shuntResistor_mOhm, uint8_t average) = 0;
// };




// 抽象基类 CurrentSensor
class CurrentSensor {
public:
    // 构造函数，初始化地址、分流电阻值和平均参数
    CurrentSensor(uint8_t address, const std::vector<float>& shuntResistorOhms, uint16_t average = 1)
        : address_(address), shuntResistorOhms_(shuntResistorOhms), average_(average) {}

    // 虚析构函数，确保派生类正确释放资源
    virtual ~CurrentSensor() = default;

    // 纯虚函数，初始化传感器
    virtual bool initialize() = 0;

    // 纯虚函数，读取传感器数据
    virtual bool readData(std::vector<currentSensorData>& data) = 0;

    // 获取总线电压 (mV)
    virtual float getBusVoltage_mV(uint8_t channel = 0) = 0;

    // 获取分流电压 (mV)
    virtual float getShuntVoltage_mV(uint8_t channel = 0) = 0;

    // 获取电流 (mA)
    virtual float getCurrent_mA(uint8_t channel = 0) = 0;

    // 获取功率 (mW)
    virtual float getPower_mW(uint8_t channel = 0) = 0;

    // 获取传感器地址
    uint8_t getAddress() const { return address_; }

    // 获取分流电阻值
    const std::vector<float>& getShuntResistorOhms() const { return shuntResistorOhms_; }

    // 设置平均参数
    void setAverage(uint16_t average) { average_ = average; }

    // 获取平均参数
    uint16_t getAverage() const { return average_; }

protected:
    uint8_t address_;                    // 传感器地址
    std::vector<float> shuntResistorOhms_;  // 各通道分流电阻值 (Ohms)
    uint16_t average_;                   // 平均采样次数
};

#endif


