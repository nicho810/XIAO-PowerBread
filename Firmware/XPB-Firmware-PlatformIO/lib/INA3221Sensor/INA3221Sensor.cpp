// This class is based on the INA3221 library by Rob Tillaart
// Author: Rob Tillaart
// Purpose: demo polling 3 channels
// URL: https://github.com/RobTillaart/INA3221_RT

#include "INA3221Sensor.h"

INA3221Sensor::INA3221Sensor(uint8_t address)
#if defined(SEEED_XIAO_RP2040)
  : ina(address, &Wire1) {
#elif defined(SEEED_XIAO_RP2350)
  : ina(address, &Wire1) {
#else
  : ina(address, &Wire) {
#endif
}

bool INA3221Sensor::begin() {
  /* when using RPI_PICO, the I2C have to set to Wire1, the actual I2C is Wire1, but on seeed_xiao_rp2040, they mapped it to Wire */
  #if defined(SEEED_XIAO_RP2040)
    Wire1.setSDA(pin_i2c_sda);
    Wire1.setSCL(pin_i2c_scl);
    Wire1.setClock(400000); // Set I2C to 400KHz
    Wire1.begin();
  #elif defined(SEEED_XIAO_RP2350)
    Wire1.setSDA(pin_i2c_sda);
    Wire1.setSCL(pin_i2c_scl);
    Wire1.setClock(1000000); // Note: 1MHz works while 400KHz doesn't work.
    Wire1.begin();
  #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32C6)
    // Wire.setSDA(pin_i2c_sda);
    // Wire.setSCL(pin_i2c_scl);
    Wire.setClock(400000); // Set I2C to 400KHz
    Wire.begin();
  #elif defined(SEEED_XIAO_ESP32S3)
    // Wire.setSDA(pin_i2c_sda);
    // Wire.setSCL(pin_i2c_scl);
    // Wire.setClock(400000); // Set I2C to 400KHz
    // Wire.setPins(pin_i2c_sda, pin_i2c_scl);
    if (!Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000)) {
      Serial.println("Failed to initialize I2C");
    }
  #endif

  if (!ina.begin()) {
    Serial.println("could not connect. Fix and Reboot");
    return false;
  }

  // setParameter(0.050, 0.050);
  
  delay(100);
  return true;
}

void INA3221Sensor::setParameter(float shuntResistorCHA, float shuntResistorCHB) {
  disableChannel(2); // Disable unused channel 2
  setShuntResistors(shuntResistorCHA, shuntResistorCHB);
  Serial.printf("> Shunt-Resistors: %f, %f\n", ina.getShuntR(0), ina.getShuntR(1));
  setAverage(1); //get 4 samples and average them
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

void INA3221Sensor::setAverage(int average) {
    ina.setAverage(average); //when average is 1, it means 4 samples are taken and averaged
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