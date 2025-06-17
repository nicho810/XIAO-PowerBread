#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include "global_variables.h"
#include <vector>
#include "CurrentSensor_INA226.h"



// Function declarations
void sensorTask(void *pvParameters);

extern SemaphoreHandle_t xSemaphore;
extern std::vector<currentSensorData> cSensorData;
extern cSensor_2ch_ina226 currentSensor_2ch;
