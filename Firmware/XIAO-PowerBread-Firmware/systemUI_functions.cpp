#include "systemUI_functions.h"
#include "ui_style.h"

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


