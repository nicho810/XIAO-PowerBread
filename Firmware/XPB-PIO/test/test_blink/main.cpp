#include <Arduino.h>

#define RGB_R 17

void setup() {
  pinMode(RGB_R, OUTPUT);
}

void loop() {
  digitalWrite(RGB_R, HIGH);
  delay(1000);
  digitalWrite(RGB_R, LOW);
  delay(1000);
}
