#include "dataChart_functions.h"
#include "ui_style.h"

// dataChart functions
void dataChart_changeRotation(int rotation) {
  tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)
}

const int CHART_WIDTH = 160;
const int CHART_HEIGHT = 63;
GFXcanvas16* chartCanvas = nullptr;  // Change to GFXcanvas16

void dataChart_initUI(uint8_t channel) {
  tft.fillScreen(color_Background);

  uint16_t color_channel = color_ChannelA;
  if (channel == 0) {
    color_channel = color_ChannelA;
  } else {
    color_channel = color_ChannelB;
  } 

  // Initialize the chartCanvas
  if (chartCanvas == nullptr) {
    chartCanvas = new GFXcanvas16(CHART_WIDTH, CHART_HEIGHT);  // Change to GFXcanvas16
  }
  chartCanvas->fillScreen(0); // Clear the canvas with black

  tft.fillRoundRect(-2, -2, 69, 16, 4, color_channel);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(5, 4);
  if (channel == 0) {
    tft.print("Channel A");
  } else if (channel == 1) {
    tft.print("Channel B");
  }

  tft.setFont(&FreeSansBold9pt7b);  // set font first in case
}

void dataChart_exitUI() {
  // Free up the memory used by chartCanvas
  if (chartCanvas != nullptr) {
    delete chartCanvas;
    chartCanvas = nullptr;
  }
}

void dataChart_updateData(const DualChannelData &sensorData, uint8_t ch) {
  if (chartCanvas == nullptr) return; // Safety check

  const uint8_t y_top = 16;
  const uint8_t y_bottom = 79;
  const uint8_t y_max = y_bottom - y_top;

  const float current_maxScale = 100.0; // mA, lowered for better visibility
  float current_mA = 0; // mA

  // Fetch new current data from sensorData based on ch
  if (ch == 0) {
    current_mA = sensorData.channel0.busCurrent;
  } else if (ch == 1) {
    current_mA = sensorData.channel1.busCurrent;
  }

  // Calculate the height of the current bar
  int16_t bar_height = (int16_t)((current_mA / current_maxScale) * CHART_HEIGHT);
  bar_height = constrain(bar_height, 0, CHART_HEIGHT);

  // Draw grid lines
  for (int i = 0; i < CHART_HEIGHT; i += CHART_HEIGHT / 4) {
    chartCanvas->drawFastHLine(0, i, CHART_WIDTH, color_GridLines);
  }

  // Shift the existing chart data to the left
  chartCanvas->drawRGBBitmap(-1, 0, chartCanvas->getBuffer(), CHART_WIDTH, CHART_HEIGHT);

  // Clear the rightmost column
  chartCanvas->drawFastVLine(CHART_WIDTH - 1, 0, CHART_HEIGHT, color_Background);

  // Draw the new data point on the rightmost column
  uint16_t lineColor;
  if (ch == 0) {
    lineColor = color_ChartChannelA;
  } else if (ch == 1) {
    lineColor = color_ChartChannelB;
  }
  chartCanvas->drawFastVLine(CHART_WIDTH - 1, CHART_HEIGHT - bar_height, bar_height, lineColor);



  // Draw the entire updated canvas on the TFT display
  tft.drawRGBBitmap(0, y_top, chartCanvas->getBuffer(), CHART_WIDTH, CHART_HEIGHT);

  // Update the current value text on the TFT display
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(color_Text, color_Background);
  tft.setCursor(80, 4);
  tft.print(current_mA, 2);
  tft.print(" mA   ");
}