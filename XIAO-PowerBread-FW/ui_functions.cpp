#include "ui_functions.h"

// Define color definitions and global variables
uint16_t color_Background = 0x0000;
uint16_t color_Text = 0xFFFF;
uint16_t color_ChannelA = 0x055B;
uint16_t color_ChannelB = 0xF97C;
uint8_t chA_x = 1;
uint8_t chA_y = 82;
uint8_t chB_x = 1;
uint8_t chB_y = 0;

void ChannelInfoUpdate_A(float new_chA_v, float new_chA_a, float new_chA_w, float old_chA_v, float old_chA_a, float old_chA_w, uint16_t color) {
  uint8_t chA_1_x = chA_x + 4;
  uint8_t chA_1_y = chA_y + 30;
  uint8_t chA_2_y = chA_y + 50;
  uint8_t chA_3_y = chA_y + 70;
  uint8_t chA_v_x = chA_x + 70;
  uint8_t chA_mA_x = chA_x + 64;
  uint8_t chA_w_x = chA_x + 64;

  int offset_defaultFont = -6;


  // Update voltage if changed
  if (new_chA_v != old_chA_v) {
    updateChangedDigits(chA_1_x, chA_1_y, old_chA_v, new_chA_v, color);
  }

  // Update current if changed
  if (new_chA_a != old_chA_a) {
    updateChangedDigits(chA_1_x, chA_2_y, old_chA_a, new_chA_a, color);
  }

  // Update power if changed
  if (new_chA_w != old_chA_w) {
    updateChangedDigits(chA_1_x, chA_3_y, old_chA_w, new_chA_w, color);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chA_v_x, chA_1_y + offset_defaultFont);
  tft.print("V");
  tft.setCursor(chA_mA_x, chA_2_y + offset_defaultFont);
  tft.print("mA");
  tft.setCursor(chA_w_x, chA_3_y + offset_defaultFont);
  tft.print("mW");
}

void ChannelInfoUpdate_B(float new_chB_v, float new_chB_a, float new_chB_w, float old_chB_v, float old_chB_a, float old_chB_w, uint16_t color) {
  uint8_t chB_1_x = chB_x + 4;
  uint8_t chB_1_y = chB_y + 30;
  uint8_t chB_2_y = chB_y + 50;
  uint8_t chB_3_y = chB_y + 70;
  uint8_t chB_v_x = chB_x + 70;
  uint8_t chB_mA_x = chB_x + 64;
  uint8_t chB_w_x = chB_x + 64;

  int offset_defaultFont = -6;


  // Update voltage if changed
  if (new_chB_v != old_chB_v) {
    updateChangedDigits(chB_1_x, chB_1_y, old_chB_v, new_chB_v, color);
  }

  // Update current if changed
  if (new_chB_a != old_chB_a) {
    updateChangedDigits(chB_1_x, chB_2_y, old_chB_a, new_chB_a, color);
  }

  // Update power if changed
  if (new_chB_w != old_chB_w) {
    updateChangedDigits(chB_1_x, chB_3_y, old_chB_w, new_chB_w, color);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chB_v_x, chB_1_y + offset_defaultFont);
  tft.print("V");
  tft.setCursor(chB_mA_x, chB_2_y + offset_defaultFont);
  tft.print("mA");
  tft.setCursor(chB_w_x, chB_3_y + offset_defaultFont);
  tft.print("mW");
}

void updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color) {
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

void drawUIFramework() {
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

  tft.setFont(&FreeSansBold9pt7b); // set font first in case
}

void changeRotation(int rotation, float old_chA_v, float old_chA_a, float old_chA_w, float old_chB_v, float old_chB_a, float old_chB_w){
  tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation); //Rotate the LCD 180 degree (0-3)
  // delay(50);
  drawUIFramework();
  ChannelInfoUpdate_A(old_chA_v,  old_chA_a,  old_chA_w,  -1,  -1,  -1);
  ChannelInfoUpdate_B(old_chB_v,  old_chB_a,  old_chB_w,  -1,  -1,  -1);
  // delay(50);
}