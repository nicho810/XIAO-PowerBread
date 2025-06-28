#include "Arduino.h"
#include "sysConfig.h"

SysConfig sysConfig;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2500);
  Serial.println("Test SysConfig");
  sysConfig.initialize(false);
  sysConfig.loadFromEEPROM();
  Serial.println("SysConfig initialized");
  Serial.println(sysConfig.toString());
}

void loop() {
}