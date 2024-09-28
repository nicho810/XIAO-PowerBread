#ifndef DATAMONITORCHART_FUNCTIONS_H
#define DATAMONITORCHART_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "INA3221Sensor.h"  // Add this line to include the DualChannelData type

extern Adafruit_ST7735 tft;

void dataMonitorChart_changeRotation(const DualChannelData &sensorData, uint8_t channel, int rotation);
void dataMonitorChart_initUI(uint8_t channel, int rotation);
void dataMonitorChart_updateData(const DualChannelData &sensorData, uint8_t ch, int rotation, uint8_t forceUpdate=0);
void dataMonitorChart_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color);

#endif // DATAMONITORCHART_FUNCTIONS_H