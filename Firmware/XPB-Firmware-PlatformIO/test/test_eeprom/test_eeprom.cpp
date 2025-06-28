#include "Arduino.h"
#include <Wire.h>

#include "SparkFun_External_EEPROM.h"
ExternalEEPROM extMem;

void setup()
{
  Serial.begin(115200);
  delay(250); //Often needed for ESP based platforms
  Serial.println("Qwiic EEPROM example");

  Wire.begin();

  //We must set the memory specs. Pick your EEPROM From the list:

  // 24xx00 - 128 bit / 16 bytes - 1 address byte, 1 byte page
  // 24xx01 - 1024 bit / 128 bytes - 1 address byte, 8 byte page
  // 24xx02 - 2048 bit / 256 bytes - 1 address byte, 8 byte page
  // 24xx04 - 4096 bit / 512 bytes - 1 address byte, 16 byte page
  // 24xx08 - 8192 bit / 1024 bytes - 1 address byte, 16 byte page
  // 24xx16 - 16384 bit / 2048 bytes - 1 address byte, 16 byte page
  // 24xx32 - 32768 bit / 4096 bytes - 2 address bytes, 32 byte page
  // 24xx64 - 65536 bit / 8192 bytes - 2 address bytes, 32 byte page
  // 24xx128 - 131072 bit / 16384 bytes - 2 address bytes, 64 byte page
  // 24xx256 - 262144 bit / 32768 bytes - 2 address bytes, 64 byte page
  // 24xx512 - 524288 bit / 65536 bytes - 2 address bytes, 128 byte page
  // 24xx1025 - 1024000 bit / 128000 byte - 2 address byte, 128 byte page
  // 24xxM02 - 2097152 bit / 262144 byte - 2 address bytes, 256 byte page

  // Setting the memory type configures the memory size in bytes, the number of address bytes, and the page size in bytes.

  // Default to the Qwiic 24xx512 EEPROM: https://www.sparkfun.com/products/18355
  extMem.setMemoryType(2); // Valid types: 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1025, 2048, we use AT24C02 here

  if (extMem.begin(0x50, Wire) == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (true)
      ;
  }
  Serial.println("Memory detected!");

  Serial.print("Mem size in bytes: ");
  Serial.println(extMem.length());

  //Yes you can read and write bytes, but you shouldn't!
  byte myValue1 = 200;
  extMem.write(0, myValue1); //(location, data)

  byte myRead1 = extMem.read(0);
  Serial.print("I read (should be 200): ");
  Serial.println(myRead1);

  //You should use gets and puts. This will automatically and correctly arrange
  //the bytes for larger variable types.
  int myValue2 = -366;
  extMem.put(10, myValue2); //(location, data)
  int myRead2;
  extMem.get(10, myRead2); //location to read, thing to put data into
  Serial.print("I read (should be -366): ");
  Serial.println(myRead2);

  float myValue3 = -7.35;
  extMem.put(20, myValue3); //(location, data)
  float myRead3;
  extMem.get(20, myRead3); //location to read, thing to put data into
  Serial.print("I read (should be -7.35): ");
  Serial.println(myRead3);

  String myString = "Hi, I am just a simple test string";
  unsigned long nextEEPROMLocation = extMem.putString(30, myString);
  String myRead4 = "";
  extMem.getString(30, myRead4);
  Serial.print("I read: ");
  Serial.println(myRead4);
  Serial.print("Next available EEPROM location: ");
  Serial.println(nextEEPROMLocation);
}

void loop()
{
}