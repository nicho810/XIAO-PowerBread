#ifndef SYSTEMUI_FUNCTIONS_H
#define SYSTEMUI_FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "ui_style.h"
#include "sysConfig.h"

extern Adafruit_ST7735 tft;

void systemUI_init();
void systemUI_bootScreen();
void systemUI_sysConfig_init();
void systemUI_sysConfig_update(int cursor, bool isSelected, sysConfig_data tmp_cfg_data);
void systemUI_MSG_savedConfig();

#endif