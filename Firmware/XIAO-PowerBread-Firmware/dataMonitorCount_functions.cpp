#include "dataMonitorCount_functions.h"


extern DualChannelData oldSensorData;

// Function definitions
void dataMonitorCount_initUI(uint8_t channel, uint8_t rotation) {
  tft.fillScreen(color_Background);

  uint16_t color_channel = color_ChannelA;
  if (channel == 0) {
    color_channel = color_ChannelA;
  } else {
    color_channel = color_ChannelB;
  }

  uint8_t dataBox_x = 0;
  uint8_t dataBox_y = 0;
  uint8_t chartBox_x = 0;
  uint8_t chartBox_y = 0;
  if (rotation == 0 || rotation == 2) {
    dataBox_x = 1;
    dataBox_y = 0;
    chartBox_x = 1;
    chartBox_y = 82;
  } else if (rotation == 1 || rotation == 3) {
    dataBox_x = 1;
    dataBox_y = 0;
    chartBox_x = 82;
    chartBox_y = 0;
  }

  //dataBox
  tft.drawRoundRect(dataBox_x, dataBox_y, 78, 78, 4, color_channel);
  tft.fillRoundRect(dataBox_x, dataBox_y, 65, 12, 4, color_channel);
  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(dataBox_x + 5, dataBox_y + 2);
  if (channel == 0) {
    tft.print("Channel A");
  } else if (channel == 1) {
    tft.print("Channel B");
  }

  //countBox
  tft.drawRoundRect(chartBox_x, chartBox_y, 78, 78, 4, color_channel);
  
  //todo
}


void dataMonitorCount_updateData(const DualChannelData &sensorData, uint8_t channel, uint8_t rotation, uint8_t forceUpdate) {
  uint8_t dataBox_x = 0;
  uint8_t dataBox_y = 0;
  uint8_t chartBox_x = 0;
  uint8_t chartBox_y = 0;
  if (rotation == 0 || rotation == 2) {
    dataBox_x = 1;
    dataBox_y = 0;
    chartBox_x = 1;
    chartBox_y = 82;
  } else if (rotation == 1 || rotation == 3) {
    dataBox_x = 1;
    dataBox_y = 0;
    chartBox_x = 82;
    chartBox_y = 0;
  }

  int offset_defaultFont = -6;

  //fetch data
  float busVoltage = 0;
  float busCurrent = 0;
  float busPower = 0;
  if (channel == 0) {
    busVoltage = sensorData.channel0.busVoltage;
    busCurrent = sensorData.channel0.busCurrent;
    busPower = sensorData.channel0.busPower;
  } else if (channel == 1) {
    busVoltage = sensorData.channel1.busVoltage;
    busCurrent = sensorData.channel1.busCurrent;
    busPower = sensorData.channel1.busPower;
  }

  //update dataBox
  uint8_t volatge_x = dataBox_x + 4;
  uint8_t volatge_y = dataBox_y + 30;
  uint8_t current_x = dataBox_x + 4;
  uint8_t current_y = dataBox_y + 50;
  uint8_t power_x = dataBox_x + 4;
  uint8_t power_y = dataBox_y + 70;

  uint8_t volatge_old = 0;
  uint8_t current_old = 0;
  uint8_t power_old = 0;
  if (channel == 0) {
    volatge_old = oldSensorData.channel0.busVoltage;
    current_old = oldSensorData.channel0.busCurrent;
    power_old = oldSensorData.channel0.busPower;
  } else if (channel == 1) {
    volatge_old = oldSensorData.channel1.busVoltage;
    current_old = oldSensorData.channel1.busCurrent;
    power_old = oldSensorData.channel1.busPower;
  }

  uint16_t lineColor = (channel == 0) ? color_ChartChannelA : color_ChartChannelB;
  uint16_t lineHighlightColor = (channel == 0) ? color_ChannelA : color_ChannelB;


  //if forceUpdate is 1, update all data anyway
  if (forceUpdate == 1) {
    dataMonitorCount_updateChangedDigits(volatge_x, volatge_y, volatge_old, busVoltage, color_Text);
    dataMonitorCount_updateChangedDigits(current_x, current_y, current_old, busCurrent, color_Text);
    dataMonitorCount_updateChangedDigits(power_x, power_y, power_old, busPower, color_Text);
  } else if (forceUpdate == 0) {
    if (busVoltage != volatge_old) {
      dataMonitorCount_updateChangedDigits(volatge_x, volatge_y, volatge_old, busVoltage, color_Text);
    }
    if (busCurrent != current_old) {
      dataMonitorCount_updateChangedDigits(current_x, current_y, current_old, busCurrent, color_Text);
    }
    if (busPower != power_old) {
      dataMonitorCount_updateChangedDigits(power_x, power_y, power_old, busPower, color_Text);
    }
  }


  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setCursor(volatge_x + 65, volatge_y + offset_defaultFont);
  tft.print("V");
  tft.setCursor(current_x + 59, current_y + offset_defaultFont);
  tft.print("mA");
  tft.setCursor(power_x + 59, power_y + offset_defaultFont);
  tft.print("mW");
}



void dataMonitorCount_changeRotation(const DualChannelData &sensorData, uint8_t channel, int rotation) {
  tft.setRotation(rotation);  // Only set rotation, don't reinitialize the display
  dataMonitorCount_initUI(channel, rotation);
  dataMonitorCount_updateData(sensorData, channel, rotation, 1);  // Force update all data
}



void dataMonitorCount_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color) {
  char newStr[10];

  // Determine the number of decimal places dynamically
  int decimalPlaces = (newValue < 10) ? 3 : (newValue < 100)  ? 2
                                          : (newValue < 1000) ? 1
                                                              : 0;

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  snprintf(newStr, sizeof(newStr), "%5.*f", decimalPlaces, newValue);

  // Get bounds of the entire string
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(newStr, x, y, &x1, &y1, &w, &h);

  // Erase the old value area
  tft.fillRect(x1 - 2, y1, w + 3, h, color_Background);

  // Draw the new value
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(newStr);
}