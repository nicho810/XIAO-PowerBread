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
        int dial_threshold_reset_min = 0;
        int dial_threshold_reset_max = 50;
        int dial_threshold_press_min = 50;
        int dial_threshold_press_max = 150;
        int dial_threshold_down_min = 150;
        int dial_threshold_down_max = 250;
        int dial_threshold_up_min = 300;
        int dial_threshold_up_max = 400;
    #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3)
        #if defined(Proj_XIAOPowerMonitor)
        int dial_threshold_reset_min = 0;
        int dial_threshold_reset_max = 200;
        int dial_threshold_press_min = 320;
        int dial_threshold_press_max = 500;
        int dial_threshold_down_min = 200;
        int dial_threshold_down_max = 320;
        int dial_threshold_up_min = 500;
        int dial_threshold_up_max = 700;
        #elif defined(Proj_XIAOPowerBread)
        int dial_threshold_reset_min = 0;
        int dial_threshold_reset_max = 100;
        int dial_threshold_press_min = 100;
        int dial_threshold_press_max = 200;
        int dial_threshold_down_min = 200;
        int dial_threshold_down_max = 350;
        int dial_threshold_up_min = 350;
        int dial_threshold_up_max = 600;
        #endif
    #elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        int dial_threshold_reset_min = 0;
        int dial_threshold_reset_max = 100;
        int dial_threshold_press_min = 100;
        int dial_threshold_press_max = 200;
        int dial_threshold_down_min = 200;
        int dial_threshold_down_max = 350;
        int dial_threshold_up_min = 350;
        int dial_threshold_up_max = 600;
    #endif

    int dialValue = read();
    
    int dialStatus_temp = 0;
    if (dialValue >= dial_threshold_up_min && dialValue < dial_threshold_up_max) {
        dialStatus_temp = 1;           // up
    }
    else if (dialValue >= dial_threshold_down_min && dialValue < dial_threshold_down_max) {
        dialStatus_temp = 2;           // down
    }
    else if (dialValue >= dial_threshold_press_min && dialValue < dial_threshold_press_max) {
        dialStatus_temp = 3;           // press
    }
    else if (dialValue >= dial_threshold_reset_min && dialValue < dial_threshold_reset_max) {
        dialStatus_temp = 0;           // reset
    }
    else {
        dialStatus_temp = 0;           // invalid range, default to reset
    }   

    // Debug print
    // Serial.println("dialValue: " + String(dialValue) + " dialStatus: " + String(dialStatus_temp));

    return dialStatus_temp;
}

