#include <Arduino.h>
#include "currentSensor.h"
#include "CurrentSensor_INA226.h"

CurrentSensor_DualINA226 c_Sensor;
DualChannelData latestSensorData; 





void setup() {
    delay(2500);
    Serial.begin(115200);
    Serial.println("Starting test...");

    // Current Sensor Init
    if (!c_Sensor.begin()){
        while (1){
            Serial.println("Current Sensor initialization failed. Please check the wiring and try again."); // Print error message
            delay(1000);
            // Todo: Add LCD error message
        }
    }
}

void loop(){
    latestSensorData = c_Sensor.readCurrentSensors();

    //print the data to the serial monitor
    Serial.printf("Channel 0: %f, Channel 1: %f\n", latestSensorData.channel0.busCurrent, latestSensorData.channel1.busCurrent);
    delay(1000);
}

