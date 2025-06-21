#pragma once

#include <Arduino.h>
#include "boardConfig.h"
#include <vector>
#include "CurrentSensor_INA226.h"


// Function declarations
void sensorTask(void *pvParameters);

// External variable declarations
extern QueueHandle_t sensorDataQueue;
extern cSensor_2ch_ina226 currentSensor_2ch;

