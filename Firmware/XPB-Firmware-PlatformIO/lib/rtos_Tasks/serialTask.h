#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "INA3221Sensor.h" // Adjust path as needed

void serialPrintTask(void *pvParameters);

#endif // SERIAL_TASK_H
