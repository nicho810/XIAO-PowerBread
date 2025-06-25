#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include <vector>
#include "CurrentSensor_INA226.h"

// Function declarations
void serialTask(void *pvParameters);
void readSerialCommands();
void printSensorData();
void processATCommand(String command);

// External variable declarations
extern QueueHandle_t sensorDataQueue_serial;

#endif // SERIAL_TASK_H
