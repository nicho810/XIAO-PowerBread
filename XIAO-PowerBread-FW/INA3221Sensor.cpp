#include "INA3221Sensor.h"

INA3221Sensor::INA3221Sensor(uint8_t address) : ina(address) {}

bool INA3221Sensor::begin() {
    return ina.begin();
}

void INA3221Sensor::setShuntResistors(float shunt0, float shunt1) {
    ina.setShuntR(0, shunt0);
    ina.setShuntR(1, shunt1);
}

DualChannelData INA3221Sensor::readCurrentSensors() {
    DualChannelData INAData;

    // Read channel 0
    INAData.channel0.busVoltage = ina.getBusVoltage(0);
    INAData.channel0.current = ina.getCurrent_mA(0);
    INAData.channel0.power = ina.getPower_mW(0);

    // Read channel 1
    INAData.channel1.busVoltage = ina.getBusVoltage(1);
    INAData.channel1.current = ina.getCurrent_mA(1);
    INAData.channel1.power = ina.getPower_mW(1);

    // Check if values have changed
    INAData.channel0.isDirty = (INAData.channel0.busVoltage != prevINAData.channel0.busVoltage) || 
                               (INAData.channel0.current != prevINAData.channel0.current) || 
                               (INAData.channel0.power != prevINAData.channel0.power);

    INAData.channel1.isDirty = (INAData.channel1.busVoltage != prevINAData.channel1.busVoltage) || 
                               (INAData.channel1.current != prevINAData.channel1.current) || 
                               (INAData.channel1.power != prevINAData.channel1.power);

    prevINAData = INAData;
    return INAData;
}