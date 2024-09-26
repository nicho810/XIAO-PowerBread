#include "INA3221Sensor.h"

INA3221Sensor::INA3221Sensor(uint8_t address)
  : ina(address) {}

bool INA3221Sensor::begin() {
  Wire.setClock(400000); // Set I2C to 400KHz
  Wire.begin();
  if (!ina.begin()) {
    Serial.println("could not connect. Fix and Reboot");
    return false;
  } else {
    Serial.println("INA3221 Found");
    disableChannel(2); // Disable unused channel 2
    setShuntResistors(0.020, 0.020); // 20 mR shunt resistors for channels 0 and 1
    setAverage(1);//get 4 samples and average them
    delay(100);
    return true;
  }
}

void INA3221Sensor::setAverage(int average) {
  ina.setAverage(average);//get 4 samples and average them
}

void INA3221Sensor::disableChannel(int channel) {
  ina.disableChannel(channel);
}

void INA3221Sensor::setShuntResistors(float shunt0, float shunt1) {
  ina.setShuntR(0, shunt0);
  ina.setShuntR(1, shunt1);
  prevINAData.channel0.shuntResistor = shunt0;
  prevINAData.channel1.shuntResistor = shunt1;
  // Print the shunt resistors
  // Serial.printf("Shunt Resistors: %f, %f\n", ina.getShuntR(0), ina.getShuntR(1));
  // Serial.printf("Shunt Resistors: %f, %f\n", prevINAData.channel0.shuntResistor, prevINAData.channel1.shuntResistor);
}

DualChannelData INA3221Sensor::readCurrentSensors() {
  DualChannelData INAData;

  //passing shuntResister value
  INAData.channel0.shuntResistor = prevINAData.channel0.shuntResistor;
  INAData.channel1.shuntResistor = prevINAData.channel1.shuntResistor;


  // Read channel 0
  INAData.channel0.busVoltage = ina.getBusVoltage(0);
  INAData.channel0.busCurrent = ina.getCurrent_mA(0);
  INAData.channel0.busPower = ina.getPower_mW(0);
  INAData.channel0.shuntVoltage = ina.getShuntVoltage_mV(0);

  // Read channel 1
  INAData.channel1.busVoltage = ina.getBusVoltage(1);
  INAData.channel1.busCurrent = ina.getCurrent_mA(1);
  INAData.channel1.busPower = ina.getPower_mW(1);
  INAData.channel1.shuntVoltage = ina.getShuntVoltage_mV(1);

  // Check if values have changed
  INAData.channel0.isDirty = (INAData.channel0.busVoltage != prevINAData.channel0.busVoltage) || 
                             (INAData.channel0.busCurrent != prevINAData.channel0.busCurrent) || 
                             (INAData.channel0.busPower != prevINAData.channel0.busPower);

  INAData.channel1.isDirty = (INAData.channel1.busVoltage != prevINAData.channel1.busVoltage) || 
                             (INAData.channel1.busCurrent != prevINAData.channel1.busCurrent) || 
                             (INAData.channel1.busPower != prevINAData.channel1.busPower);

  prevINAData = INAData;
  return INAData;
}