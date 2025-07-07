// //
// //    FILE: INA226_demo.ino
// //  AUTHOR: Rob Tillaart
// // PURPOSE: demo
// //     URL: https://github.com/RobTillaart/INA226


// #include "INA226.h"


// INA226 INA(0x40);


// void setup()
// {
//   Serial.begin(115200);
//   Serial.println(__FILE__);
//   Serial.print("INA226_LIB_VERSION: ");
//   Serial.println(INA226_LIB_VERSION);

//   Wire.begin();
//   if (!INA.begin() )
//   {
//     Serial.println("could not connect. Fix and Reboot");
//   }
//   INA.setMaxCurrentShunt(8, 0.010);
// }


// void loop()
// {
//   Serial.println("\nBUS\tSHUNT\tCURRENT\tPOWER");
//   for (int i = 0; i < 20; i++)
//   {
//     Serial.print(INA.getBusVoltage(), 3);
//     Serial.print("\t");
//     Serial.print(INA.getShuntVoltage_mV(), 3);
//     Serial.print("\t");
//     Serial.print(INA.getCurrent_mA(), 3);
//     Serial.print("\t");
//     Serial.print(INA.getPower_mW(), 3);
//     Serial.println();
//     delay(1000);
//   }
// }


//  -- END OF FILE --






#include "CurrentSensor_INA226.h"
#include <Arduino.h>
#include <vector>
#include "boardConfig.h"



// cSensor_1ch currentSensor_1ch(currentSensor_0_address, currentSensor_0_shuntR_0, 4);
cSensor_2ch_ina226 currentSensor_2ch(currentSensor_0_address, currentSensor_1_address, {currentSensor_0_shuntR_0, currentSensor_1_shuntR_0}, {currentSensor_0_MaxCurrent_0, currentSensor_1_MaxCurrent_0}, 1);




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


