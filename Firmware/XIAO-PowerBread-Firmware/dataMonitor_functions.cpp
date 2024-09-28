#include "dataMonitor_functions.h"

// Define global variables
uint8_t chA_x = 1;
uint8_t chA_y = 82;
uint8_t chB_x = 1;
uint8_t chB_y = 0;

// Add these declarations at the top of the file if they're not already present
extern float old_chA_v, old_chA_a, old_chA_w;
extern float old_chB_v, old_chB_a, old_chB_w;

// Add the function
void dataMonitor_updateData(const DualChannelData &sensorData) {
  if (sensorData.channel0.isDirty) {
    dataMonitor_ChannelInfoUpdate(0,
                                  sensorData.channel0.busVoltage,
                                  sensorData.channel0.busCurrent,
                                  sensorData.channel0.busPower,
                                  old_chA_v, old_chA_a, old_chA_w,
                                  color_Text);
    old_chA_v = sensorData.channel0.busVoltage;
    old_chA_a = sensorData.channel0.busCurrent;
    old_chA_w = sensorData.channel0.busPower;
  }
  if (sensorData.channel1.isDirty) {
    dataMonitor_ChannelInfoUpdate(1,
                                  sensorData.channel1.busVoltage,
                                  sensorData.channel1.busCurrent,
                                  sensorData.channel1.busPower,
                                  old_chB_v, old_chB_a, old_chB_w,
                                  color_Text);
    old_chB_v = sensorData.channel1.busVoltage;
    old_chB_a = sensorData.channel1.busCurrent;
    old_chB_w = sensorData.channel1.busPower;
  }
}

void dataMonitor_ChannelInfoUpdate(uint8_t channel, float new_v, float new_a, float new_w, float old_v, float old_a, float old_w, uint16_t color=color_Text) {
  uint8_t ch_x = 0;
  uint8_t ch_y = 0;
  if (channel == 0) {
    ch_x = chA_x;
    ch_y = chA_y;
  } else {
    ch_x = chB_x;
    ch_y = chB_y;
  }  
  uint8_t ch_1_x = ch_x + 4;
  uint8_t ch_1_y = ch_y + 30;
  uint8_t ch_2_y = ch_y + 50;
  uint8_t ch_3_y = ch_y + 70;
  uint8_t ch_v_x = ch_x + 70;
  uint8_t ch_mA_x = ch_x + 64;
  uint8_t ch_w_x = ch_x + 64;

  int offset_defaultFont = -6;

  // Update voltage if changed
  if (new_v != old_v) {
    dataMonitor_updateChangedDigits(ch_1_x, ch_1_y, old_v, new_v, color);
  }

  // Update current if changed
  if (new_a != old_a) {
    dataMonitor_updateChangedDigits(ch_1_x, ch_2_y, old_a, new_a, color);
  }

  // Update power if changed
  if (new_w != old_w) {
    dataMonitor_updateChangedDigits(ch_1_x, ch_3_y, old_w, new_w, color);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(ch_v_x, ch_1_y + offset_defaultFont);
  tft.print("V");
  tft.setCursor(ch_mA_x, ch_2_y + offset_defaultFont);
  tft.print("mA");
  tft.setCursor(ch_w_x, ch_3_y + offset_defaultFont);
  tft.print("mW");
}



void dataMonitor_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color) {
  char oldStr[10], newStr[10];
  char format[10];

  // Determine the number of decimal places automatically
  int decimalPlaces = 3;  // Default to 3 decimal places
  if (oldValue >= 10 || newValue >= 10) {
    decimalPlaces = 2;
  } else if (oldValue >= 100 || newValue >= 100) {
    decimalPlaces = 1;
  } else if (oldValue >= 1000 || newValue >= 1000) {
    decimalPlaces = 0;
  }

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  snprintf(format, sizeof(format), "%%5.%df", decimalPlaces);
  snprintf(oldStr, sizeof(oldStr), format, oldValue);
  snprintf(newStr, sizeof(newStr), format, newValue);

  // Get bounds of the entire string
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(oldStr, x, y, &x1, &y1, &w, &h);

  // Erase the entire old string
  tft.fillRect(x1, y1, w, h, color_Background);

  // Draw the new string
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(newStr);
}

void dataMonitor_initUI(int rotation) {
  dataMonitor_update_chAB_xy_by_Rotation(rotation);
  tft.fillScreen(color_Background);

  tft.drawRoundRect(chA_x, chA_y, 78, 78, 4, color_ChannelA);
  tft.fillRoundRect(chA_x, chA_y, 65, 12, 4, color_ChannelA);

  tft.drawRoundRect(chB_x, chB_y, 78, 78, 4, color_ChannelB);
  tft.fillRoundRect(chB_x, chB_y, 65, 12, 4, color_ChannelB);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(chA_x + 5, chA_y + 2);
  tft.print("Channel A");
  tft.setCursor(chB_x + 5, chB_y + 2);
  tft.print("Channel B");

  tft.setFont(&FreeSansBold9pt7b);  // set font first in case
}

void dataMonitor_changeRotation(int rotation, float old_chA_v, float old_chA_a, float old_chA_w, float old_chB_v, float old_chB_a, float old_chB_w) {
  // tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)
  // delay(50);
  dataMonitor_initUI(rotation);
  dataMonitor_ChannelInfoUpdate(0, old_chA_v, old_chA_a, old_chA_w, -1, -1, -1, color_Text);
  dataMonitor_ChannelInfoUpdate(1, old_chB_v, old_chB_a, old_chB_w, -1, -1, -1, color_Text);
  // delay(50);
}

void dataMonitor_update_chAB_xy_by_Rotation(int rotation) {
  if (rotation == 0) {
    chA_x = 1;
    chA_y = 0;
    chB_x = 1;
    chB_y = 82;
  }
  if (rotation == 2) {
    chA_x = 1;
    chA_y = 82;
    chB_x = 1;
    chB_y = 0;
  }
  if (rotation == 1) {
    chA_x = 0;
    chA_y = 1;
    chB_x = 82;
    chB_y = 1;
  }
  if (rotation == 3) {
    chA_x = 82;
    chA_y = 1;
    chB_x = 0;
    chB_y = 1;
  }
}
