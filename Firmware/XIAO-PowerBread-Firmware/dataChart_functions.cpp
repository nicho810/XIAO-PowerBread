#include "dataChart_functions.h"


#include "ui_style.h"

// dataChart functions
void dataChart_changeRotation(int rotation) {
  tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)

}


void dataChart_initUI(uint8_t channel) {
  tft.fillScreen(color_Background);

  uint16_t color_channel = color_ChannelA;
  if (channel == 0) {
    color_channel = color_ChannelA;
  } else {
    color_channel = color_ChannelB;
  } 

//   tft.drawRoundRect(0, 1, 160, 78, 4, color_channel);
  tft.drawRect(0, 0, 160, 78, color_channel);
  tft.fillRoundRect(-2, -2, 69, 16, 4, color_channel);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(5, 2);
  if (channel == 0) {
    tft.print("Channel A");
  } else if (channel == 1) {
    tft.print("Channel B");
  }

  tft.setFont(&FreeSansBold9pt7b);  // set font first in case
}

void dataChart_updateData(const DualChannelData &sensorData, uint8_t ch) {
// tft.fillScreen(color_Background);
// tft.setCursor(0, tft.height() / 2);
// tft.print("Data Chart");

uint8_t y_top = 16;
uint8_t y_bottom = 79;
uint8_t y_max = y_bottom-y_top;


unsigned int current_maxScale = 1000; //mA
float current_mA = 0;//mA

//fetch new current data from sensorData base on ch
// if (ch == 0) {
//   float current_mA = sensorData.channel0.busCurrent;
// } else if (ch == 1) {
//   float current_mA = sensorData.channel1.busCurrent;
// }

float length_of_current_mA = current_mA / current_maxScale * y_max;
uint8_t length_of_current_mA_int = (uint8_t)length_of_current_mA;//float to int

tft.drawFastVLine(80, y_bottom-length_of_current_mA_int, length_of_current_mA_int, color_Text);
  
  // Add your chart drawing logic here
  // You can use sensorData.channel0 or sensorData.channel1 depending on the ch parameter
}