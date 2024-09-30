#ifndef SYSTEMUI_FUNCTIONS_H
#define SYSTEMUI_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "ui_style.h"

extern Adafruit_ST7735 tft;

void systemUI_init();
void systemUI_bootScreen();

#endif