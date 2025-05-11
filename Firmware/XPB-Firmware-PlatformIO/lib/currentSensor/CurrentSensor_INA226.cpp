// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226

#include "CurrentSensor_INA226.h"

CurrentSensor_INA226::CurrentSensor_INA226(uint8_t address)
#if defined(SEEED_XIAO_RP2040)
  : ina226(address, &Wire1) {
#elif defined(SEEED_XIAO_RP2350)
  : ina226(address, &Wire1) {
#else
  : ina226(address, &Wire) {
#endif
}

bool CurrentSensor_INA226::begin() {
  if (!ina226.begin()) {
    Serial.println("could not connect INA226. Fix and Reboot");
    return false;
  }
  
  delay(100);
  return true;
}

void CurrentSensor_INA226::setParameter(float shuntResistor) {
  setShuntResistors(shuntResistor);
  Serial.printf("> Shunt-Resistors: %f\n", ina226.getShunt());
  setAverage(1); //get 4 samples and average them
}

void CurrentSensor_INA226::setShuntResistors(float shuntResistor) {
  ina226.setMaxCurrentShunt(20.0, shuntResistor);
}

void CurrentSensor_INA226::setAverage(int average) {
    ina226.setAverage(average); //when average is 1, it means 4 samples are taken and averaged
}



CurrentSensor_DualINA226::CurrentSensor_DualINA226(uint8_t address1, uint8_t address2){
    ina226_ch1 = CurrentSensor_INA226(address1);
    ina226_ch2 = CurrentSensor_INA226(address2);
}


bool CurrentSensor_DualINA226::begin() {
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
    // Wire.setClock(400000); // Set I2C to 400KHz
    // Wire.begin();
    Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
  #elif defined(SEEED_XIAO_ESP32S3)
    Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
  #endif

  // Initialize the INA226 sensors
  if (!ina226_ch1.begin()) {
    Serial.println("INA226_CH1 initialization failed");
    return false;
  }
  if (!ina226_ch2.begin()) {
    Serial.println("INA226_CH2 initialization failed");
    return false;
  }
  return true;
}

void CurrentSensor_DualINA226::setParameter(float shuntResistor1, float shuntResistor2) {
  ina226_ch1.setParameter(shuntResistor1);
  ina226_ch2.setParameter(shuntResistor2);
}


DualChannelData CurrentSensor_DualINA226::readCurrentSensors() {
  DualChannelData INAData;

  //passing shuntResister value
  INAData.channel0.shuntResistor = prevINAData.channel0.shuntResistor;
  INAData.channel1.shuntResistor = prevINAData.channel1.shuntResistor;


  // Read channel 0
  INAData.channel0.busVoltage = ina226_ch1.ina226.getBusVoltage();
  INAData.channel0.busCurrent = ina226_ch1.ina226.getCurrent_mA();
  INAData.channel0.busPower = ina226_ch1.ina226.getPower_mW();
  INAData.channel0.shuntVoltage = ina226_ch1.ina226.getShuntVoltage_mV();

  // Read channel 1
  INAData.channel1.busVoltage = ina226_ch2.ina226.getBusVoltage();
  INAData.channel1.busCurrent = ina226_ch2.ina226.getCurrent_mA();
  INAData.channel1.busPower = ina226_ch2.ina226.getPower_mW();
  INAData.channel1.shuntVoltage = ina226_ch2.ina226.getShuntVoltage_mV();

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