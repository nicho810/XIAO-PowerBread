#ifndef DATAMONITOR_FUNCTIONS_H
#define DATAMONITOR_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include "../tft_driver/XPB_ST7735.h"
#include "../INA3221Sensor.h"  // Add this include
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "ui_style.h"


extern Adafruit_ST7735 tft;


// Function declarations
void dataMonitor_ChannelInfoUpdate(uint8_t channel, float new_v, float new_a, float new_w, float old_v, float old_a, float old_w, uint16_t color=color_Text, uint8_t forceUpdate=0);
void dataMonitor_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color);
void dataMonitor_initUI(int rotation);
void dataMonitor_changeRotation(int rotation, const DualChannelData &oldSensorData);
void dataMonitor_update_chAB_xy_by_Rotation(int rotation);
void dataMonitor_updateData(const DualChannelData &sensorData, uint8_t forceUpdate=0);

#endif // DATAMONITOR_FUNCTIONS_H

