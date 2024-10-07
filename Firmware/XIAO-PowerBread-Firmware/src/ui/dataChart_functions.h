#ifndef DATACHART_FUNCTIONS_H
#define DATACHART_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include "../tft_driver/XPB_ST7735.h"
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../INA3221Sensor.h"  // Add this line to include the DualChannelData type
#include "ui_style.h"

extern Adafruit_ST7735 tft;

void dataChart_changeRotation(int rotation);
void dataChart_initUI(uint8_t channel);
void dataChart_updateData(const DualChannelData &sensorData, uint8_t ch);
void dataChart_exitUI();

#endif // DATACHART_FUNCTIONS_H