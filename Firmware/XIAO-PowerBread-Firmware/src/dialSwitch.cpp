#include "dialSwitch.h"
#define dial_pin A2

void DialFunction::init() {
  pinMode(dial_pin, INPUT);
  analogReadResolution(12);  // Set the ADC resolution to 12 bits
}

int DialFunction::read() {
  return analogRead(dial_pin);
}

int DialFunction::readDialStatus() {
    //dial analog value:
    //0-299: 0 -> reset
    //300-799: 1 -> up
    //800-1499: 2 -> down
    //1500-1999: 3 -> press

    //dial status:
    //0: reset
    //1: up
    //2: down
    //3: press
    //4: long press

    int dialValue = read();
    int dialStatus_temp = 0;
    if (dialValue >= 300 && dialValue < 800) {
        dialStatus_temp = 3;
    }
    else if (dialValue >= 800 && dialValue < 1500) {
        dialStatus_temp = 2;
    }
    else if (dialValue >= 1500 && dialValue < 2000) {
        dialStatus_temp = 1;
    }
    else {
        dialStatus_temp = 0;
    }   
    return dialStatus_temp;
}

