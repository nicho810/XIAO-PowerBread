#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

extern Adafruit_ST7735 tft;

// Declare color definitions and global variables
extern uint16_t color_Background;
extern uint16_t color_Text;
extern uint16_t color_ChannelA;
extern uint16_t color_ChannelB;
extern uint8_t chA_x, chA_y, chB_x, chB_y;

// Function declarations
void ChannelInfoUpdate_A(float new_chA_v, float new_chA_a, float new_chA_w, float old_chA_v, float old_chA_a, float old_chA_w, uint16_t color = color_Text);
void ChannelInfoUpdate_B(float new_chB_v, float new_chB_a, float new_chB_w, float old_chB_v, float old_chB_a, float old_chB_w, uint16_t color = color_Text);
void updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color);
void drawUIFramework();

#endif // UI_FUNCTIONS_H