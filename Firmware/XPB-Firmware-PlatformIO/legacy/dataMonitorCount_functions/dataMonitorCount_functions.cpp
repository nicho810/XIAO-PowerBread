#include "dataMonitorCount_functions.h"


extern DualChannelData oldSensorData;

// Add these global variables at the top of the file
extern float avgS[2], avgM[2], avgA[2], peak[2];
extern unsigned long dataCount[2];
extern unsigned long lastUpdate;
extern double sumS[2], sumM[2], sumA[2];  // Changed to double for higher precision
extern unsigned long countS[2], countM[2];  // Changed to unsigned long for larger range

// Constants for overflow prevention
const unsigned long MAX_COUNT = 1000000000;  // 1 billion samples
const double MAX_SUM = 1.0e12;  // 1 trillion

// Function definitions
void dataMonitorCount_initUI(uint8_t channel, uint8_t rotation) {
  tft.fillScreen(color_Background);

  uint16_t color_channel = color_ChannelA;
  uint16_t color_channel_highlight = color_ChartChannelA;
  if (channel == 0) {
    color_channel = color_ChannelA;
    color_channel_highlight = color_ChartChannelA;
  } else {
    color_channel = color_ChannelB;
    color_channel_highlight = color_ChartChannelB;
  }

  uint8_t dataBox_x = 0;
  uint8_t dataBox_y = 0;
  uint8_t countBox_x = 0;
  uint8_t countBox_y = 0;
  if (rotation == 0 || rotation == 2) {
    dataBox_x = 1;
    dataBox_y = 0;
    countBox_x = 1;
    countBox_y = 82;
  } else if (rotation == 1 || rotation == 3) {
    dataBox_x = 1;
    dataBox_y = 0;
    countBox_x = 82;
    countBox_y = 0;
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
  tft.drawRoundRect(dataBox_x, dataBox_y, 78, 78, 4, color_channel);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  const char* countBox_text[4] = {"AvgS", "AvgM", "AvgA", "Peak"};

  //AvgS = Average current over 1 second
  //AvgM = Average current over 1 minute
  //AvgA = Average current since enter this mode
  //Peak = Peak current

  const uint8_t boxWidth = 78;
  const uint8_t boxHeight = 18;
  const uint8_t cornerRadius = 4;
  const uint8_t highlightWidth = 28;

  for (int i = 0; i <= 3; i++) {
    uint8_t yPos = countBox_y + (i * 20);
    tft.fillRoundRect(countBox_x, yPos, highlightWidth, boxHeight, cornerRadius, color_channel_highlight);
    tft.fillRect(countBox_x + highlightWidth -3, yPos, 4, boxHeight, color_channel_highlight); //fill the right top and bottom corner of the highlight
    tft.drawRoundRect(countBox_x, yPos, boxWidth, boxHeight, cornerRadius, color_channel);
    tft.setCursor(countBox_x + 3, yPos + 5);
    tft.print(countBox_text[i]);
  }
}


void dataMonitorCount_updateData(const DualChannelData &sensorData, uint8_t channel, uint8_t rotation, uint8_t forceUpdate) {
  uint8_t dataBox_x = 0;
  uint8_t dataBox_y = 0;
  uint8_t countBox_x = 0;
  uint8_t countBox_y = 0;
  if (rotation == 0 || rotation == 2) {
    dataBox_x = 1;
    dataBox_y = 0;
    countBox_x = 1;
    countBox_y = 82;
  } else if (rotation == 1 || rotation == 3) {
    dataBox_x = 1;
    dataBox_y = 0;
    countBox_x = 82;
    countBox_y = 0;
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


  //update the countBox: AvgS, AvgM, AvgA, Peak
  // AvgS = Average current over 1 second
  // AvgM = Average current over 1 minute
  // AvgA = Average current since enter this mode
  // Peak = Peak current

  // Update averages and peak
  unsigned long currentTime = millis();
  float currentValue = busCurrent;

  // Accumulate data for averages
  sumS[channel] += currentValue;
  sumM[channel] += currentValue;
  sumA[channel] += currentValue;
  countS[channel]++;
  countM[channel]++;
  dataCount[channel]++;

  // Prevent overflow
  if (countS[channel] >= MAX_COUNT || sumS[channel] >= MAX_SUM) {
    avgS[channel] = sumS[channel] / countS[channel];
    sumS[channel] = avgS[channel];
    countS[channel] = 1;
  }
  if (countM[channel] >= MAX_COUNT || sumM[channel] >= MAX_SUM) {
    avgM[channel] = sumM[channel] / countM[channel];
    sumM[channel] = avgM[channel];
    countM[channel] = 1;
  }
  if (dataCount[channel] >= MAX_COUNT || sumA[channel] >= MAX_SUM) {
    avgA[channel] = sumA[channel] / dataCount[channel];
    sumA[channel] = avgA[channel];
    dataCount[channel] = 1;
  }

  // Update all averages every 100ms
  if (currentTime - lastUpdate >= 100) {
    for (int i = 0; i < 2; i++) {
      if (countS[i] > 0) {
        avgS[i] = sumS[i] / countS[i];
        sumS[i] = 0;
        countS[i] = 0;
      }
      if (countM[i] > 0) {
        // For AvgM, we'll use an exponential moving average
        float alpha = 2.0 / 601.0;  // Smoothing factor for last ~60 seconds
        avgM[i] = alpha * currentValue + (1 - alpha) * avgM[i];
      }
      if (dataCount[i] > 0) {
        avgA[i] = sumA[i] / dataCount[i];
      }
    }
    lastUpdate = currentTime;

    // Update peak
    if (currentValue > peak[channel]) {
      peak[channel] = currentValue;
    }

    // Display the updated data
    tft.setFont();
    tft.setTextColor(color_Text, color_Background);
    tft.setTextSize(0);
    char countBox_text[4][10];
    snprintf(countBox_text[0], sizeof(countBox_text[0]), "%7.2f", avgS[channel]);
    snprintf(countBox_text[1], sizeof(countBox_text[1]), "%7.2f", avgM[channel]);
    snprintf(countBox_text[2], sizeof(countBox_text[2]), "%7.2f", avgA[channel]);
    snprintf(countBox_text[3], sizeof(countBox_text[3]), "%7.2f", peak[channel]);

    for (int i = 0; i <= 3; i++) {
      uint8_t yPos = countBox_y + (i * 20) + 5;
      tft.setCursor(countBox_x + 30, yPos);
      tft.print(countBox_text[i]);
    }
  }
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