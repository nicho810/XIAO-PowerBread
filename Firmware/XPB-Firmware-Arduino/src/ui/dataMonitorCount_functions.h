#ifndef DATAMONITORCOUNT_FUNCTIONS_H
#define DATAMONITORCOUNT_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include "../tft_driver/XPB_ST7735.h"
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../INA3221Sensor.h"
#include "ui_style.h"

extern Adafruit_ST7735 tft;

// Function declarations only
void dataMonitorCount_initUI(uint8_t channel, uint8_t rotation);
void dataMonitorCount_updateData(const DualChannelData &sensorData, uint8_t channel, uint8_t rotation, uint8_t forceUpdate = 0);
void dataMonitorCount_changeRotation(const DualChannelData &sensorData, uint8_t channel, int rotation); 
void dataMonitorCount_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color = color_Text);

#endif // DATAMONITORCOUNT_FUNCTIONS_H
