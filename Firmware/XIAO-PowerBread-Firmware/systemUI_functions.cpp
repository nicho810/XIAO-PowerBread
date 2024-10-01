#include "systemUI_functions.h"

void systemUI_init() {
    tft.fillScreen(color_Background);
}

void systemUI_bootScreen() {
    tft.setCursor(0, 0);
    tft.setTextColor(color_Text);
    tft.setTextSize(0);
    tft.setFont();
    tft.print("Booting...");
}


void systemUI_sysConfig_init(){
    uint16_t color_sysConfig = color_ChannelA;  
    tft.fillScreen(color_Background);
    tft.fillRoundRect(0, 0, 80, 12, 4, color_sysConfig);

    tft.setFont();
    tft.setTextColor(color_Text);
    tft.setTextSize(0);
    tft.setCursor(4, 2);
    tft.print("SysConfig V1");

    const char* config_text[8] = {"Dft Mode", "Dft CH", "ShuntR CHA", "ShuntR CHB", "Serial EN", "Serial BR", "Serial Mode", "Chart ITl"};
    tft.setTextColor(color_Text, color_Background);
    for (int i = 0; i < 8; i++) {
        tft.setCursor(0, 20 + i * 14);
        tft.print(config_text[i]);
    }

}