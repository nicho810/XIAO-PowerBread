#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include <vector>
#include "CurrentSensor_INA226.h"
#include "sysConfig.h"
#include "ATCommand.h"

// Function declarations
void serialTask(void *pvParameters);
void readSerialCommands();
void printSensorData();
void processATCommand(String command);

// External variable declarations
extern QueueHandle_t sensorDataQueue_serial;

// External variable declarations
extern SysConfig sysConfig;
extern ATCommand atCommand;

#endif // SERIAL_TASK_H
