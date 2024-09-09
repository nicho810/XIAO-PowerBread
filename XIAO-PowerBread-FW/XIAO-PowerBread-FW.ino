//LCD init
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

#define TFT_CS -1  //CS is always connected to ground in this project.
#define TFT_RST D3
#define TFT_DC D9
#define TFT_MOSI D10
#define TFT_SCLK D8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define color_Background 0x0000
#define color_Text 0xFFFF
#define color_ChannelA 0x055B
#define color_ChannelB 0xF97C

//Current sensor init
#include "INA3221.h"
INA3221 INA(0x40);

struct INAData {
  float busVoltage;
  float current;
  float power;
  bool isDirty; // Add this flag
};

struct DualChannelData {
  INAData channel0;
  INAData channel1;
};

float old_chA_v = 0, old_chA_a = 0, old_chA_w = 0;
float old_chB_v = 0, old_chB_a = 0, old_chB_w = 0;

// Add these global variables near the top of your file, with other global declarations
uint8_t chA_x = 1;
uint8_t chA_y = 82;
uint8_t chB_x = 1;
uint8_t chB_y = 0;

DualChannelData readCurrentSensors() {
  static DualChannelData prevINAData;
  DualChannelData INAData;
  
  // Read channel 0
  INAData.channel0.busVoltage = INA.getBusVoltage(0);
  INAData.channel0.current = INA.getCurrent_mA(0);
  INAData.channel0.power = INA.getPower_mW(0);
  
  // Read channel 1
  INAData.channel1.busVoltage = INA.getBusVoltage(1);
  INAData.channel1.current = INA.getCurrent_mA(1);
  INAData.channel1.power = INA.getPower_mW(1);
  
  // Check if values have changed
  INAData.channel0.isDirty = (INAData.channel0.busVoltage != prevINAData.channel0.busVoltage) ||
                             (INAData.channel0.current != prevINAData.channel0.current) ||
                             (INAData.channel0.power != prevINAData.channel0.power);
  
  INAData.channel1.isDirty = (INAData.channel1.busVoltage != prevINAData.channel1.busVoltage) ||
                             (INAData.channel1.current != prevINAData.channel1.current) ||
                             (INAData.channel1.power != prevINAData.channel1.power);
  
  prevINAData = INAData;
  return INAData;
}



// void clean_channelInfoArea_A() {
//   tft.fillRect(5, 14, 58, 60, color_Background);
// }

// void clean_channelInfoArea_B() {
//   tft.fillRect(5, 96, 58, 60, color_Background);
// }

void ChannelInfoUpdate_A(float new_chA_v, float new_chA_a, float new_chA_w, float old_chA_v, float old_chA_a, float old_chA_w, uint16_t color = color_Text) {
  uint8_t chA_1_x = chA_x + 4;
  uint8_t chA_1_y = chA_y + 30;
  uint8_t chA_2_y = chA_y + 50;
  uint8_t chA_3_y = chA_y + 70;
  uint8_t chA_v_x = chA_x + 70;
  uint8_t chA_mA_x = chA_x + 64;
  uint8_t chA_w_x = chA_x + 64;

  int offset_defaultFont = -6;

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Update voltage if changed
  if (new_chA_v != old_chA_v) {
    tft.setTextColor(color_Background);
    tft.setCursor(chA_1_x, chA_1_y);
    tft.printf("%4.3f", old_chA_v);
    tft.setTextColor(color);
    tft.setCursor(chA_1_x, chA_1_y);
    tft.printf("%4.3f", new_chA_v);
  }

  // Update current if changed
  if (new_chA_a != old_chA_a) {
    tft.setTextColor(color_Background);
    tft.setCursor(chA_1_x, chA_2_y);
    printFormattedValue(old_chA_a);
    tft.setTextColor(color);
    tft.setCursor(chA_1_x, chA_2_y);
    printFormattedValue(new_chA_a);
  }

  // Update power if changed
  if (new_chA_w != old_chA_w) {
    tft.setTextColor(color_Background);
    tft.setCursor(chA_1_x, chA_3_y);
    printFormattedValue(old_chA_w);
    tft.setTextColor(color);
    tft.setCursor(chA_1_x, chA_3_y);
    printFormattedValue(new_chA_w);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chA_v_x, chA_1_y + offset_defaultFont);
  tft.println("V");
  tft.setCursor(chA_mA_x, chA_2_y + offset_defaultFont);
  tft.println("mA");
  tft.setCursor(chA_w_x, chA_3_y + offset_defaultFont);
  tft.println("mW");
}

void ChannelInfoUpdate_B(float new_chB_v, float new_chB_a, float new_chB_w, float old_chB_v, float old_chB_a, float old_chB_w, uint16_t color = color_Text) {
  uint8_t chB_1_x = chB_x + 4;
  uint8_t chB_1_y = chB_y + 30;
  uint8_t chB_2_y = chB_y + 50;
  uint8_t chB_3_y = chB_y + 70;
  uint8_t chB_v_x = chB_x + 70;
  uint8_t chB_mA_x = chB_x + 64;
  uint8_t chB_w_x = chB_x + 64;

  int offset_defaultFont = -6;

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Update voltage if changed
  if (new_chB_v != old_chB_v) {
    tft.setTextColor(color_Background);
    tft.setCursor(chB_1_x, chB_1_y);
    tft.printf("%4.3f", old_chB_v);
    tft.setTextColor(color);
    tft.setCursor(chB_1_x, chB_1_y);
    tft.printf("%4.3f", new_chB_v);
  }

  // Update current if changed
  if (new_chB_a != old_chB_a) {
    tft.setTextColor(color_Background);
    tft.setCursor(chB_1_x, chB_2_y);
    printFormattedValue(old_chB_a);
    tft.setTextColor(color);
    tft.setCursor(chB_1_x, chB_2_y);
    printFormattedValue(new_chB_a);
  }

  // Update power if changed
  if (new_chB_w != old_chB_w) {
    tft.setTextColor(color_Background);
    tft.setCursor(chB_1_x, chB_3_y);
    printFormattedValue(old_chB_w);
    tft.setTextColor(color);
    tft.setCursor(chB_1_x, chB_3_y);
    printFormattedValue(new_chB_w);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chB_v_x, chB_1_y + offset_defaultFont);
  tft.println("V");
  tft.setCursor(chB_mA_x, chB_2_y + offset_defaultFont);
  tft.println("mA");
  tft.setCursor(chB_w_x, chB_3_y + offset_defaultFont);
  tft.println("mW");
}

void printFormattedValue(float value) {
  if (value > 9999) {
    tft.print("9999+");
  } else if (value >= 1000) {
    tft.printf("%4.0f", value);
  } else if (value >= 100) {
    tft.printf("%4.1f", value);
  } else if (value >= 10) {
    tft.printf("%4.2f", value);
  } else {
    tft.printf("%4.3f", value);
  }
}

void drawUIFramework() {
  tft.drawRoundRect(chA_x, chA_y, 78, 78, 4, color_ChannelA);
  tft.fillRoundRect(chA_x, chA_y, 65, 12, 4, color_ChannelA);

  tft.drawRoundRect(chB_x, chB_y, 78, 78, 4, color_ChannelB);
  tft.fillRoundRect(chB_x, chB_y, 65, 12, 4, color_ChannelB);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(chA_x + 5, chA_y + 2);
  tft.println("Channel A");
  tft.setCursor(chB_x + 5, chB_y + 2);
  tft.println("Channel B");
}


void setup(void) {
  // init serial
  Serial.begin(115200);
  Serial.print(F("Hello! XIAO PowerBread."));

  // LCD init
  tft.initR(INITR_GREENTAB);  // Init ST7735S 0.96inch display (160*80), Also need to modify the _colstart = 24 and _rowstart = 0 in Adafruit_ST7735.cpp>initR(uint8_t)
  tft.setRotation(2);         //Rotate the LCD 180 degree (0-3)
  tft.setSPISpeed(62500000);  //50MHz
  tft.fillScreen(color_Background);

  //UI init
  drawUIFramework();
  // ChannelInfoUpdate_A(0, 0, 0);
  // ChannelInfoUpdate_B(0, 0, 0);

  //Current sensor init
  Wire.begin();
  if (!INA.begin()) {
    Serial.println("could not connect. Fix and Reboot");
  } else {
    Serial.print("Found: \t");
    Serial.println(INA.getAddress());
  }
  INA.setShuntR(0, 0.100);
  INA.setShuntR(1, 0.100);
  Wire.setClock(400000);//400 kHz
  delay(100);  // fllussssh IO
}


void loop() {
    DualChannelData sensorData = readCurrentSensors();
    
    if (sensorData.channel0.isDirty) {
        ChannelInfoUpdate_A(
            sensorData.channel0.busVoltage, 
            sensorData.channel0.current, 
            sensorData.channel0.power,
            old_chA_v, old_chA_a, old_chA_w
        );
        // Update old values for next iteration
        old_chA_v = sensorData.channel0.busVoltage;
        old_chA_a = sensorData.channel0.current;
        old_chA_w = sensorData.channel0.power;
    }
    
    if (sensorData.channel1.isDirty) {
        ChannelInfoUpdate_B(
            sensorData.channel1.busVoltage, 
            sensorData.channel1.current, 
            sensorData.channel1.power,
            old_chB_v, old_chB_a, old_chB_w
        );
        // Update old values for next iteration
        old_chB_v = sensorData.channel1.busVoltage;
        old_chB_a = sensorData.channel1.current;
        old_chB_w = sensorData.channel1.power;
    }
    
    delay(2);
}
