#include "dataMonitorChart_functions.h"


const int smallChart_WIDTH = 74;
const int smallChart_HEIGHT = 74;
std::unique_ptr<GFXcanvas16> smallChartCanvas;  // Change to GFXcanvas16

extern DualChannelData oldSensorData;

float chartScale = 100.0;

void dataMonitorChart_changeRotation(const DualChannelData &sensorData, uint8_t channel, int rotation) {
  tft.setRotation(rotation);  // Only set rotation, don't reinitialize the display
  dataMonitorChart_initUI(channel, rotation);
  dataMonitorChart_updateData(sensorData, channel, rotation, 1); // Force update all data
}


void dataMonitorChart_initUI(uint8_t channel, int rotation) {
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

  //chartBox
  tft.drawRoundRect(chartBox_x, chartBox_y, 78, 78, 4, color_channel);
  smallChartCanvas.reset(new GFXcanvas16(smallChart_WIDTH, smallChart_HEIGHT));
  smallChartCanvas->fillScreen(0); // Clear the canvas with black
}

void dataMonitorChart_updateData(const DualChannelData &sensorData, uint8_t ch, int rotation, uint8_t forceUpdate) {
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
  if (ch == 0) {
    busVoltage = sensorData.channel0.busVoltage;
    busCurrent = sensorData.channel0.busCurrent;
    busPower = sensorData.channel0.busPower;
  } else if (ch == 1) {
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
  if (ch == 0) {
    volatge_old = oldSensorData.channel0.busVoltage;
    current_old = oldSensorData.channel0.busCurrent;
    power_old = oldSensorData.channel0.busPower;
  } else if (ch == 1) {
    volatge_old = oldSensorData.channel1.busVoltage;
    current_old = oldSensorData.channel1.busCurrent;
    power_old = oldSensorData.channel1.busPower;
  }

  uint16_t lineColor = (ch == 0) ? color_ChartChannelA : color_ChartChannelB;
  uint16_t lineHighlightColor = (ch == 0) ? color_ChannelA : color_ChannelB;


  //if forceUpdate is 1, update all data anyway
  if (forceUpdate == 1) {
    dataMonitorChart_updateChangedDigits(volatge_x, volatge_y, volatge_old, busVoltage, color_Text);
    dataMonitorChart_updateChangedDigits(current_x, current_y, current_old, busCurrent, color_Text);
    dataMonitorChart_updateChangedDigits(power_x, power_y, power_old, busPower, color_Text);
  } else if (forceUpdate == 0) {
    if (busVoltage != volatge_old) {
      dataMonitorChart_updateChangedDigits(volatge_x, volatge_y, volatge_old, busVoltage, color_Text);
    }
    if (busCurrent != current_old) {
      dataMonitorChart_updateChangedDigits(current_x, current_y, current_old, busCurrent, color_Text);
    }
    if (busPower != power_old) {
      dataMonitorChart_updateChangedDigits(power_x, power_y, power_old, busPower, color_Text);
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

  //update chartBox
  if (!smallChartCanvas) return; // Safety check

  //Adjust the max scale of the chart based on the current value
  float current_maxScale = chartScale; // mA, default scale
  if (busCurrent >= 1800) {
    current_maxScale = 5000.0;
  } else if (busCurrent >= 800) {
    current_maxScale = 2000.0;
  } else if (busCurrent >= 400) {
    current_maxScale = 1000.0;
  } else if (busCurrent > 80) {
    current_maxScale = 500.0;
  } else if (busCurrent <= 80) {
    current_maxScale = 100.0;
  }

  //draw a V line to indicate the scale is changed
  if (chartScale != current_maxScale) {
    smallChartCanvas->drawFastVLine(smallChart_WIDTH - 1, 0, smallChart_HEIGHT, lineHighlightColor);
    chartScale = current_maxScale;
  }

  // Calculate the height of the current bar    
  int16_t bar_height = (int16_t)((busCurrent / current_maxScale) * smallChart_HEIGHT);
  bar_height = constrain(bar_height, 0, smallChart_HEIGHT);

  // Initialize horizontal grid lines if the canvas is empty
  if (smallChartCanvas->getPixel(0, 0) == color_Background) {
    for (int y = 0; y < smallChart_HEIGHT; y += smallChart_HEIGHT / 4) {
      smallChartCanvas->drawFastHLine(0, y, smallChart_WIDTH, color_GridLines);
    }
  }

  // Shift existing data to the left
  smallChartCanvas->drawRGBBitmap(-1, 0, smallChartCanvas->getBuffer(), smallChart_WIDTH, smallChart_HEIGHT);

  // Clear the rightmost column
  smallChartCanvas->drawFastVLine(smallChart_WIDTH - 1, 0, smallChart_HEIGHT, color_Background);

  // Redraw horizontal grid lines for the rightmost column
  for (int y = 0; y < smallChart_HEIGHT; y += smallChart_HEIGHT / 4) {
    smallChartCanvas->drawPixel(smallChart_WIDTH - 1, y, color_GridLines);
  }

  //draw the new data
  smallChartCanvas->drawFastVLine(smallChart_WIDTH - 1, smallChart_HEIGHT - bar_height, bar_height, lineColor);
  smallChartCanvas->drawFastVLine(smallChart_WIDTH - 1, smallChart_HEIGHT - bar_height, 2, lineHighlightColor);

  // Draw the entire updated canvas on the TFT display
  tft.drawRGBBitmap(chartBox_x + 2, chartBox_y + 2, smallChartCanvas->getBuffer(), smallChart_WIDTH, smallChart_HEIGHT); 

}

void dataMonitorChart_exitUI() {
  smallChartCanvas.reset();
}


void dataMonitorChart_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color = color_Text) {
  char newStr[10];
  
  // Determine the number of decimal places dynamically
  int decimalPlaces = (newValue < 10) ? 3 : (newValue < 100) ? 2 : (newValue < 1000) ? 1 : 0;
  
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  snprintf(newStr, sizeof(newStr), "%5.*f", decimalPlaces, newValue);

  // Get bounds of the entire string
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(newStr, x, y, &x1, &y1, &w, &h);

  // Erase the old value area
  tft.fillRect(x1-2, y1, w+3, h, color_Background);

  // Draw the new value
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(newStr);
}