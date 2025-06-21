#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include <cstdint>
#include <vector>

struct currentSensorData {
  float busVoltage_mV;
  float shuntVoltage_mV;
  float current_mA;
  float power_mW;
};

// Queue message structure
struct SensorDataMessage {
    // std::vector<currentSensorData> data;  //This is likely to cause a stack overflow, use a fixed size array instead.
    currentSensorData data[2]; //Adjust the size of the array if needed when use multiple channels sensors.
    uint32_t timestamp;
};



// Abstract base class CurrentSensor
class CurrentSensor {
public:
    // Constructor, initialize address, shunt resistor values and averaging parameters
    CurrentSensor(uint8_t address, const std::vector<float>& shuntResistorOhms, uint16_t average = 1)
        : address_(address), shuntResistorOhms_(shuntResistorOhms), average_(average) {}

    // Virtual destructor to ensure proper resource cleanup in derived classes
    virtual ~CurrentSensor() = default;

    // Pure virtual function to initialize sensor
    virtual bool initialize() = 0;

    // Pure virtual function to read sensor data
    virtual bool readData(std::vector<currentSensorData>& data) = 0;

    // Get bus voltage (mV)
    virtual float getBusVoltage_mV(uint8_t channel = 0) = 0;

    // Get shunt voltage (mV)
    virtual float getShuntVoltage_mV(uint8_t channel = 0) = 0;

    // Get current (mA)
    virtual float getCurrent_mA(uint8_t channel = 0) = 0;

    // Get power (mW)
    virtual float getPower_mW(uint8_t channel = 0) = 0;

    // Get sensor address
    uint8_t getAddress() const { return address_; }

    // Get shunt resistor values
    const std::vector<float>& getShuntResistorOhms() const { return shuntResistorOhms_; }

    // Set averaging parameter
    void setAverage(uint16_t average) { average_ = average; }

    // Get averaging parameter
    uint16_t getAverage() const { return average_; }

protected:
    uint8_t address_;                    // Sensor address
    std::vector<float> shuntResistorOhms_;  // Shunt resistor values for each channel (Ohms)
    uint16_t average_;                   // Number of samples to average
};

#endif

