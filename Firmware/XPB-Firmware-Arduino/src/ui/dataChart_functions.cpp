#include "dataChart_functions.h"

// dataChart functions
void dataChart_changeRotation(int rotation) {
  tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)
}

const int CHART_WIDTH = 158;
const int CHART_HEIGHT = 62;
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

  tft.fillRoundRect(-2, -2, 36, 13, 4, color_channel);
  tft.drawRect(0, 0, 160, 80, color_channel);


  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(5, 2);
  if (channel == 0) {
    tft.print("CH A");
  } else if (channel == 1) {
    tft.print("CH B");
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

  const uint8_t y_top = 17;
  const uint8_t y_bottom = 79;
  const uint8_t y_max = y_bottom - y_top;

  const float current_maxScale = 100.0; // mA, lowered for better visibility
  float current_mA = 0; // mA
  float current_V = 0; // V

  // Fetch new current data from sensorData based on ch
  if (ch == 0) {
    current_mA = sensorData.channel0.busCurrent;
    current_V = sensorData.channel0.busVoltage;
  } else if (ch == 1) {
    current_mA = sensorData.channel1.busCurrent;
    current_V = sensorData.channel1.busVoltage;
  }

  // Calculate the height of the current bar
  int16_t bar_height = (int16_t)((current_mA / current_maxScale) * CHART_HEIGHT);
  bar_height = constrain(bar_height, 0, CHART_HEIGHT);

  // Initialize horizontal grid lines if the canvas is empty
  if (chartCanvas->getPixel(0, 0) == color_Background) {
    for (int y = 0; y < CHART_HEIGHT; y += CHART_HEIGHT / 4) {
      chartCanvas->drawFastHLine(0, y, CHART_WIDTH, color_GridLines);
    }
  }

  // Shift existing data to the left
  chartCanvas->drawRGBBitmap(-1, 0, chartCanvas->getBuffer(), CHART_WIDTH, CHART_HEIGHT);

  // Clear the rightmost column
  chartCanvas->drawFastVLine(CHART_WIDTH - 1, 0, CHART_HEIGHT, color_Background);

  // Redraw horizontal grid lines for the rightmost column
  for (int y = 0; y < CHART_HEIGHT; y += CHART_HEIGHT / 4) {
    chartCanvas->drawPixel(CHART_WIDTH - 1, y, color_GridLines);
  }

  // Draw the new data point
  uint16_t lineColor = (ch == 0) ? color_ChartChannelA : color_ChartChannelB;
  uint16_t lineHighlightColor = (ch == 0) ? color_ChannelA : color_ChannelB;

  chartCanvas->drawFastVLine(CHART_WIDTH - 1, CHART_HEIGHT - bar_height, bar_height, lineColor);
  chartCanvas->drawFastVLine(CHART_WIDTH - 1, CHART_HEIGHT - bar_height, 2, lineHighlightColor);

  // Draw the entire updated canvas on the TFT display
  tft.drawRGBBitmap(1, y_top, chartCanvas->getBuffer(), CHART_WIDTH, CHART_HEIGHT);

  // Update the current value text on the TFT display
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(color_Text, color_Background);
  tft.setCursor(45, 3);
  tft.print(current_V, 2);
  tft.print("V  ");
  tft.setCursor(90, 3);
  tft.print(current_mA, 2);
  tft.print(" mA  ");//since the text is left aligned, add extra space to clear the right side
}