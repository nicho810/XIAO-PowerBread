#ifndef DATACHART_FUNCTIONS_H
#define DATACHART_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

extern Adafruit_ST7735 tft;

void dataChart_changeRotation(int rotation);

#endif // DATACHART_FUNCTIONS_H