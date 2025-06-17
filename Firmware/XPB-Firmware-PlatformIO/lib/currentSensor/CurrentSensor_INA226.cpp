// This class is based on the INA226 library by Rob Tillaart
// Author: Rob Tillaart
// URL: https://github.com/RobTillaart/INA226

#include "CurrentSensor_INA226.h"

// 单通道传感器实现

cSensor_1ch::cSensor_1ch(uint8_t address, float shuntResistorOhm, uint16_t average)
    : CurrentSensor(address, {shuntResistorOhm}, average), ina226_(address) {
    if (shuntResistorOhm <= 0) {
        Serial.println("Shunt resistor value must be positive");
    }
}

bool cSensor_1ch::initialize() {
    if (!ina226_.begin()) {
        Serial.print("Failed to initialize ina226 at address 0x");
        Serial.print(static_cast<int>(address_), HEX);
        Serial.println();
        return false;
    }
    // 配置平均参数（ina226 库不支持直接设置平均次数，这里仅打印）
    Serial.print("cSensor_1ch initialized at address 0x");
    Serial.print(static_cast<int>(address_), HEX);
    Serial.print(" with average: ");
    Serial.println(average_);
    return true;
}

bool cSensor_1ch::readData(std::vector<currentSensorData>& data) {
    data.clear();
    currentSensorData sensorData;
    sensorData.busVoltage_mV = ina226_.getBusVoltage_mV();
    sensorData.current_mA = ina226_.getCurrent_mA();
    sensorData.power_mW = ina226_.getPower_mW();
    sensorData.shuntVoltage_mV = ina226_.getShuntVoltage_mV();
    data.push_back(sensorData);
    return true;
}

float cSensor_1ch::getBusVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getBusVoltage_mV();
}

float cSensor_1ch::getShuntVoltage_mV(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getShuntVoltage_mV();
}

float cSensor_1ch::getCurrent_mA(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getCurrent_mA();
}

float cSensor_1ch::getPower_mW(uint8_t channel) {
    if (channel != 0) {
        Serial.println("cSensor_1ch has only one channel (index 0)");
    }
    return ina226_.getPower_mW();
}

// 双通道传感器实现

cSensor_2ch::cSensor_2ch(uint8_t address1, uint8_t address2, const std::vector<float>& shuntResistorOhms, uint16_t average)
    : CurrentSensor(address1, shuntResistorOhms, average) {
    if (shuntResistorOhms.size() != 2) {
        Serial.println("cSensor_2ch requires exactly 2 shunt resistor values");
    }
    for (float r : shuntResistorOhms) {
        if (r <= 0) {
            Serial.println("Shunt resistor values must be positive");
        }
    }
    // Use the two provided addresses
    addresses_ = {address1, address2};
    ina226s_ = {INA226(address1), INA226(address2)};
}

bool cSensor_2ch::initialize() {
    for (size_t i = 0; i < ina226s_.size(); ++i) {
        if (!ina226s_[i].begin()) {
            Serial.print("Failed to initialize ina226 at address 0x");
            Serial.print(static_cast<int>(addresses_[i]), HEX);
            Serial.println();
            return false;
        }
    }
    Serial.print("cSensor_2ch initialized at base address 0x");
    Serial.print(static_cast<int>(address_), HEX);
    Serial.print(" with average: ");
    Serial.println(average_);
    return true;
}

bool cSensor_2ch::readData(std::vector<currentSensorData>& data) {
    data.clear();
    for (size_t i = 0; i < ina226s_.size(); ++i) {
        currentSensorData sensorData;
        sensorData.busVoltage_mV = ina226s_[i].getBusVoltage_mV();
        sensorData.shuntVoltage_mV = ina226s_[i].getShuntVoltage_mV();
        sensorData.current_mA = ina226s_[i].getCurrent_mA();
        sensorData.power_mW = ina226s_[i].getPower_mW();
        data.push_back(sensorData);
    }
    return true;
}

float cSensor_2ch::getBusVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getBusVoltage_mV();
}

float cSensor_2ch::getShuntVoltage_mV(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getShuntVoltage_mV();
}

float cSensor_2ch::getCurrent_mA(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getCurrent_mA();
}

float cSensor_2ch::getPower_mW(uint8_t channel) {
    if (channel >= ina226s_.size()) {
        Serial.println("Invalid channel index for cSensor_2ch");
    }
    return ina226s_[channel].getPower_mW();
}

// CurrentSensor_INA226::CurrentSensor_INA226(uint8_t address)
// #if defined(SEEED_XIAO_RP2040)
//   : ina226(address, &Wire1) {
// #elif defined(SEEED_XIAO_RP2350)
//   : ina226(address, &Wire1) {
// #else
//   : ina226(address, &Wire) {
// #endif
// }

// bool CurrentSensor_INA226::begin() {
//   if (!ina226.begin()) {
//     Serial.println("could not connect INA226. Fix and Reboot");
//     return false;
//   }
//   Serial.printf("Init INA226(address: 0x%02X) success.\n", ina226.getAddress());

//   delay(100);
//   return true;
// }

// void CurrentSensor_INA226::setParameter(float shuntResistor) {
//   setShuntResistors(shuntResistor);
//   Serial.printf("> Shunt-Resistors: %f\n", ina226.getShunt());
//   setAverage(1); //get 4 samples and average them
// }

// void CurrentSensor_INA226::setShuntResistors(float shuntResistor) {
//   ina226.setMaxCurrentShunt(20.0, shuntResistor);
// }

// void CurrentSensor_INA226::setAverage(int average) {
//     ina226.setAverage(average); //when average is 1, it means 4 samples are taken and averaged
// }



// CurrentSensor_DualINA226::CurrentSensor_DualINA226(uint8_t address1, uint8_t address2){
//     ina226_ch1 = CurrentSensor_INA226(address1);
//     ina226_ch2 = CurrentSensor_INA226(address2);
// }


// bool CurrentSensor_DualINA226::begin() {
//   /* when using RPI_PICO, the I2C have to set to Wire1, the actual I2C is Wire1, but on seeed_xiao_rp2040, they mapped it to Wire */
//   #if defined(SEEED_XIAO_RP2040)
//     Wire1.setSDA(pin_i2c_sda);
//     Wire1.setSCL(pin_i2c_scl);
//     Wire1.setClock(400000); // Set I2C to 400KHz
//     Wire1.begin();
//   #elif defined(SEEED_XIAO_RP2350)
//     Wire1.setSDA(pin_i2c_sda);
//     Wire1.setSCL(pin_i2c_scl);
//     Wire1.setClock(1000000); // Note: 1MHz works while 400KHz doesn't work.
//     Wire1.begin();
//   #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32C6)
//     // Wire.setSDA(pin_i2c_sda);
//     // Wire.setSCL(pin_i2c_scl);
//     // Wire.setClock(400000); // Set I2C to 400KHz
//     // Wire.begin();
//     Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
//   #elif defined(SEEED_XIAO_ESP32S3)
//     Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
//   #endif

//   // Initialize the INA226 sensors
//   if (!ina226_ch1.begin()) {
//     Serial.println("INA226_CH1 initialization failed");
//     return false;
//   }
//   if (!ina226_ch2.begin()) {
//     Serial.println("INA226_CH2 initialization failed");
//     return false;
//   }
//   return true;
// }

// void CurrentSensor_DualINA226::setParameter(float shuntResistor1, float shuntResistor2) {
//   ina226_ch1.setParameter(shuntResistor1);
//   ina226_ch2.setParameter(shuntResistor2);
// }


// DualChannelData CurrentSensor_DualINA226::readCurrentSensors() {
//   DualChannelData INAData;

//   //passing shuntResister value
//   INAData.channel0.shuntResistor = prevINAData.channel0.shuntResistor;
//   INAData.channel1.shuntResistor = prevINAData.channel1.shuntResistor;


//   // Read channel 0
//   INAData.channel0.busVoltage = ina226_ch1.ina226.getBusVoltage();
//   INAData.channel0.busCurrent = ina226_ch1.ina226.getCurrent_mA();
//   INAData.channel0.busPower = ina226_ch1.ina226.getPower_mW();
//   INAData.channel0.shuntVoltage = ina226_ch1.ina226.getShuntVoltage_mV();

//   // Read channel 1
//   INAData.channel1.busVoltage = ina226_ch2.ina226.getBusVoltage();
//   INAData.channel1.busCurrent = ina226_ch2.ina226.getCurrent_mA();
//   INAData.channel1.busPower = ina226_ch2.ina226.getPower_mW();
//   INAData.channel1.shuntVoltage = ina226_ch2.ina226.getShuntVoltage_mV();

//   // Check if values have changed
//   INAData.channel0.isDirty = (INAData.channel0.busVoltage != prevINAData.channel0.busVoltage) || 
//                              (INAData.channel0.busCurrent != prevINAData.channel0.busCurrent) || 
//                              (INAData.channel0.busPower != prevINAData.channel0.busPower);

//   INAData.channel1.isDirty = (INAData.channel1.busVoltage != prevINAData.channel1.busVoltage) || 
//                              (INAData.channel1.busCurrent != prevINAData.channel1.busCurrent) || 
//                              (INAData.channel1.busPower != prevINAData.channel1.busPower);

//   prevINAData = INAData;
//   return INAData;
// }