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
void dataMonitor_ChannelInfoUpdate(uint8_t channel, float new_v, float new_a, float new_w, float old_v, float old_a, float old_w, uint16_t color);
void dataMonitor_updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color);
void dataMonitor_initUI();
void dataMonitor_changeRotation(int rotation, float old_chA_v, float old_chA_a, float old_chA_w, float old_chB_v, float old_chB_a, float old_chB_w);
void dataMonitor_update_chAB_xy_by_Rotation(int rotation);
void dataChart_changeRotation(int rotation);

#endif // UI_FUNCTIONS_H