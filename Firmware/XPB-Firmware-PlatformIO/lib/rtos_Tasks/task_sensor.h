#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include <vector>
#include "CurrentSensor_INA226.h"



// Function declarations
void sensorTask(void *pvParameters);

extern SemaphoreHandle_t dataMutex;
extern std::vector<currentSensorData> cSensorData;
extern cSensor_2ch_ina226 currentSensor_2ch;
