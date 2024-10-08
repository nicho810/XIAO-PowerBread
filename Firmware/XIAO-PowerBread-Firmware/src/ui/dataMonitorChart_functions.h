#ifndef DATAMONITORCHART_FUNCTIONS_H
#define DATAMONITORCHART_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include "../tft_driver/XPB_ST7735.h"
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../INA3221Sensor.h"  // Add this line to include the DualChannelData type
#include "ui_style.h"
#include <memory>
#include <FreeRTOS.h>
#include <task.h>
#include "../sysConfig.h"

extern Adafruit_ST7735 tft;

void dataMonitorChart_changeRotation(const DualChannelData &sensorData, uint8_t channel, int rotation, const sysConfig_data &cfg_data);
void dataMonitorChart_initUI(uint8_t channel, int rotation, const sysConfig_data& cfg_data);
void dataMonitorChart_updateData(const DualChannelData &sensorData, uint8_t ch, int rotation, const sysConfig_data &cfg_data, uint8_t forceUpdate=0);
void dataMonitorChart_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color);
void dataMonitorChart_updateChartInterval(const sysConfig_data& cfg_data);

#endif // DATAMONITORCHART_FUNCTIONS_H