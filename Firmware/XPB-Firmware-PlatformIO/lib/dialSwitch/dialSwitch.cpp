#include "dialSwitch.h"


void DialFunction::init() {
  pinMode(pin_dial, INPUT);
  analogReadResolution(10); //10 bit ADC resolution
}

int DialFunction::read() {
  return analogRead(pin_dial);
}

int DialFunction::readDialStatus() {
    #if defined(SEEED_XIAO_ESP32C6)
        /*
         * Dial Analog Value Table on XIAO ESP32-C6
         * ----------------------
         * Range     | Status  | Value | ADC Reading
         * 0-50      | reset   | 0     | ~0
         * 50-150    | press   | 3     | ~116
         * 150-250   | down    | 2     | ~204
         * 300-400   | up      | 1     | ~348
         */
        int dial_threshold_min = 0;
        int dial_threshold_down = 150;     // down ~204
        int dial_threshold_press = 50;     // press ~116
        int dial_threshold_up = 300;       // up ~348
        int dial_threshold_max = 400;
    #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3)
        /*
         * Dial Analog Value Table on XIAO ESP32-C3 or XIAO ESP32-S3
         * ----------------------
         * Range     | Status  | Value | ADC Reading C3  | ADC Reading S3
         * 0-100     | reset   | 0     | ~0              | ~0
         * 100-200   | press   | 3     | ~159            | ~140
         * 200-350   | down    | 2     | ~282            | ~247
         * 350-600   | up      | 1     | ~490            | ~433
         */
        int dial_threshold_min = 0;
        int dial_threshold_down = 200;      // down ~282
        int dial_threshold_press = 100;     // press ~159  
        int dial_threshold_up = 350;        // up ~490
        int dial_threshold_max = 600;
    #elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        /*
         * Dial Analog Value Table on XIAO RP2040 or XIAO RP2350
         * ----------------------
         * Range     | Status  | Value | ADC Reading RP2040  | ADC Reading RP2350
         * 0-100     | reset   | 0     | ~0                  | ~0
         * 100-200   | press   | 3     | ~146                | ~143
         * 200-350   | down    | 2     | ~255                | ~252
         * 350-600   | up      | 1     | ~436                | ~436
         */
        int dial_threshold_min = 0;
        int dial_threshold_down = 200;      // down ~282
        int dial_threshold_press = 100;     // press ~159  
        int dial_threshold_up = 350;        // up ~490
        int dial_threshold_max = 600;
    #endif

    int dialValue = read();
    
    int dialStatus_temp = 0;
    if (dialValue >= dial_threshold_up && dialValue < dial_threshold_max) {
        dialStatus_temp = 1;           // up
    }
    else if (dialValue >= dial_threshold_down && dialValue < dial_threshold_up) {
        dialStatus_temp = 2;           // down
    }
    else if (dialValue >= dial_threshold_press && dialValue < dial_threshold_down) {
        dialStatus_temp = 3;           // press
    }
    else {
        dialStatus_temp = 0;           // reset
    }   

    // Debug print
    // Serial.println("dialValue: " + String(dialValue) + " dialStatus: " + String(dialStatus_temp));

    return dialStatus_temp;
}

