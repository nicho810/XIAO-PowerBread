#include "CurrentSensor_INA226.h"
#include <Arduino.h>
#include <vector>
#include "boardConfig.h"



// cSensor_1ch currentSensor_1ch(currentSensor_0_address, currentSensor_0_shuntR_0, 4);
cSensor_2ch_ina226 currentSensor_2ch(currentSensor_0_address, currentSensor_1_address, {currentSensor_0_shuntR_0, currentSensor_1_shuntR_0}, 4);




void setup() {
    Serial.begin(115200);

    Wire.begin();
    // currentSensor_1ch.initialize();
    currentSensor_2ch.initialize();


    // cSensor_2ch cSensor_2ch(0x40, 0x41, {0.1f, 0.2f}, 4);
}

void loop() {

    // //Print the data of cSensor_1ch via Serial
    // std::vector<currentSensorData> singleData;
    // if (currentSensor_1ch.readData(singleData)) {
    //     Serial.println("cSensor(" + String(currentSensor_0_address, HEX) + "): busVol: " + String(singleData[0].busVoltage_mV) + " mV, shuntVol: " + String(singleData[0].shuntVoltage_mV) + " mV, current: " + String(singleData[0].current_mA) + " mA, power: " + String(singleData[0].power_mW) + " mW");
    // }

    std::vector<currentSensorData> dualData;
    if (currentSensor_2ch.readData(dualData)) {
        Serial.println("cSensor(0x" + String(currentSensor_0_address, HEX) + "): busVol: " + String(dualData[0].busVoltage_mV) + " mV, shuntVol: " + String(dualData[0].shuntVoltage_mV) + " mV, current: " + String(dualData[0].current_mA) + " mA, power: " + String(dualData[0].power_mW) + " mW");
        Serial.println("cSensor(0x" + String(currentSensor_1_address, HEX) + "): busVol: " + String(dualData[1].busVoltage_mV) + " mV, shuntVol: " + String(dualData[1].shuntVoltage_mV) + " mV, current: " + String(dualData[1].current_mA) + " mA, power: " + String(dualData[1].power_mW) + " mW");
    }



    delay(1000);

}