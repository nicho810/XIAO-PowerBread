#include "dataMonitor_functions.h"


extern DualChannelData oldSensorData;


// Update dual channel data
void dataMonitor_updateData(const DualChannelData &sensorData) {
  // Always update both channels, regardless of isDirty flag
  dataMonitor_ChannelInfoUpdate(0,
                                sensorData.channel0.busVoltage,
                                sensorData.channel0.busCurrent,
                                sensorData.channel0.busPower,
                                oldSensorData.channel0.busVoltage,
                                oldSensorData.channel0.busCurrent, 
                                oldSensorData.channel0.busPower,
                                color_Text);

  dataMonitor_ChannelInfoUpdate(1,
                                sensorData.channel1.busVoltage,
                                sensorData.channel1.busCurrent,
                                sensorData.channel1.busPower,
                                oldSensorData.channel1.busVoltage, 
                                oldSensorData.channel1.busCurrent, 
                                oldSensorData.channel1.busPower,
                                color_Text);

  // Update oldSensorData
  oldSensorData = sensorData;
}

void dataMonitor_ChannelInfoUpdate(uint8_t channel, float new_v, float new_a, float new_w, float old_v, float old_a, float old_w, uint16_t color, uint8_t forceUpdate) {
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
  char newStr[10];
  
  // Determine the number of decimal places dynamically
  // Consider both old and new values to maintain consistency
  float maxValue = max(abs(oldValue), abs(newValue));
  int decimalPlaces = (maxValue < 10) ? 3 : (maxValue < 100) ? 2 : (maxValue < 1000) ? 1 : 0;
  
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Format string for maximum possible width (e.g., "9999.9")
  char maxWidthStr[] = "9999.9";
  
  // Get bounds of the maximum width string
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(maxWidthStr, x, y, &x1, &y1, &w, &h);

  // Erase the maximum possible area
  tft.fillRect(x1-2, y1, w+3, h, color_Background);

  // Format the new value
  snprintf(newStr, sizeof(newStr), "%5.*f", decimalPlaces, newValue);

  // Draw the new value
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

void dataMonitor_changeRotation(int rotation, const DualChannelData &oldSensorData) {
  // tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)
  // delay(50);
  dataMonitor_initUI(rotation);
  dataMonitor_ChannelInfoUpdate(0, oldSensorData.channel0.busVoltage, oldSensorData.channel0.busCurrent, oldSensorData.channel0.busPower, -1, -1, -1, color_Text, 1); //force update all data
  dataMonitor_ChannelInfoUpdate(1, oldSensorData.channel1.busVoltage, oldSensorData.channel1.busCurrent, oldSensorData.channel1.busPower, -1, -1, -1, color_Text, 1); //force update all data
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
