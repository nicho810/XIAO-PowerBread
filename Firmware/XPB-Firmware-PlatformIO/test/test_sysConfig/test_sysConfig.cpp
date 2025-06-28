#include "Arduino.h"
#include "sysConfig.h"

SysConfig sysConfig;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2500);
  Serial.println("Test SysConfig");
  sysConfig.initialize(true);
  sysConfig.loadFromEEPROM();
  Serial.println("SysConfig initialized");
  Serial.println(sysConfig.toString());


  //Test setConfigValue max length
  sysConfig.setConfigValue(ConfigIndex::WIFI_SSID, "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");//26+6=32
  sysConfig.setConfigValue(ConfigIndex::WIFI_PASSWORD, "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");//26+10=32

  //Test saveToEEPROM
  sysConfig.saveToEEPROM();
  Serial.println("SysConfig saved to EEPROM");

  //Test loadFromEEPROM
  sysConfig.loadFromEEPROM();
  Serial.println("SysConfig loaded from EEPROM");
  Serial.println("SysConfig setConfigValue");
  Serial.println(sysConfig.toString());

  //Test getConfigValue
  String wifiSSID;
  String wifiPassword;
  sysConfig.getConfigValue(ConfigIndex::WIFI_SSID, wifiSSID);
  sysConfig.getConfigValue(ConfigIndex::WIFI_PASSWORD, wifiPassword);
  Serial.println("SysConfig getConfigValue");
  Serial.println(wifiSSID);
  Serial.println(wifiPassword);
}

void loop() {
}