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

void systemUI_MSG_savedConfig() {
    tft.fillScreen(color_Background);
    tft.setCursor(0, 60);
    tft.setTextColor(color_Text);
    tft.setTextSize(0);
    tft.setFont();
    tft.print("Config Saved");
}



void systemUI_sysConfig_init() {
    uint16_t color_sysConfig = color_ChannelA;  
    tft.fillScreen(color_Background);
    tft.fillRoundRect(0, 0, 80, 12, 4, color_sysConfig);

    tft.setFont();
    tft.setTextColor(color_Text);
    tft.setTextSize(0);
    tft.setCursor(4, 2);
    tft.print("SysConfig V1");

    const char* config_text[8] = {"Dft Mode", "Dft CH", "ShuntR A", "ShuntR B", "UART EN", "UART BR", "UART Mode", "Chart IT"};
    tft.setTextColor(color_Text, color_Background);
    for (int i = 0; i < 8; i++) {
        tft.setCursor(0, 20 + i * 14);
        tft.print(config_text[i]);
    }

}

void systemUI_sysConfig_update(int cursor, bool isSelected, sysConfig_data tmp_cfg_data) {
    static int lastCursor = -1;
    static bool lastIsSelected = false;
    uint16_t color_sysConfig = color_ChannelA;  

    tft.setTextColor(color_Text, color_Background);
    const char* config_value[8];
    char buffer[8][10];  // Buffer to hold converted strings

    // Convert uint8_t values to strings
    snprintf(buffer[0], 10, "%d", tmp_cfg_data.default_mode);
    snprintf(buffer[1], 10, "%d", tmp_cfg_data.default_channel);
    snprintf(buffer[2], 10, "%d", tmp_cfg_data.shuntResistorCHA);
    snprintf(buffer[3], 10, "%d", tmp_cfg_data.shuntResistorCHB);
    snprintf(buffer[4], 10, "%d", tmp_cfg_data.serial_enable);
    snprintf(buffer[5], 10, "%d", tmp_cfg_data.serial_baudRate);
    snprintf(buffer[6], 10, "%d", tmp_cfg_data.serial_mode);
    snprintf(buffer[7], 10, "%d", tmp_cfg_data.chart_updateInterval);

    // Assign converted strings to config_value array
    for (int i = 0; i < 8; i++) {
        config_value[i] = buffer[i];
    }

    for (int i = 0; i < 8; i++) {
        // Clear the area before printing the new value
        tft.fillRect(59, 20 + i * 14, 23, 8, color_Background);
        tft.setCursor(59, 20 + i * 14);
        tft.print(config_value[i]);
    }

    // Clear only the last cursor position if it has changed
    if (lastCursor != cursor || lastIsSelected != isSelected) {
        if (lastCursor >= 0) {
            tft.drawRoundRect(55, 18 + lastCursor * 14, 24, 12, 4, color_Background);
            tft.drawRoundRect(54, 17 + lastCursor * 14, 26, 14, 4, color_Background);
        }
    }

    // Draw new cursor
    if (isSelected) {
        tft.drawRoundRect(55, 18 + cursor * 14, 24, 12, 4, color_sysConfig);
        tft.drawRoundRect(54, 17 + cursor * 14, 26, 14, 4, color_sysConfig);
    } else {
        tft.drawRoundRect(55, 18 + cursor * 14, 24, 12, 4, color_Text);
        tft.drawRoundRect(54, 17 + cursor * 14, 26, 14, 4, color_Text);
    }

    // Update last cursor position and selection state
    lastCursor = cursor;
    lastIsSelected = isSelected;
}

