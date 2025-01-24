#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include "boardConfig.h"
#include "INA3221Sensor.h" // Adjust path as needed

void serialPrintTask(void *pvParameters);

#endif // SERIAL_TASK_H
