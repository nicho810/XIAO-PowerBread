#include <Arduino.h>
#include <Wire.h>
#include "boardConfig.h"

// AT24C02 EEPROM configuration
#define EEPROM_I2C_ADDRESS 0x50
#define EEPROM_SIZE 256        // 256 bytes for AT24C02
#define EEPROM_PAGE_SIZE 8     // AT24C02 has 8-byte pages

// Function prototypes
void setupI2C();
bool detectEEPROM(uint8_t address);
void scanI2CDevices();
bool readEEPROMByte(uint8_t address, uint8_t memoryAddress, uint8_t& data);
bool readEEPROMBytes(uint8_t address, uint8_t memoryAddress, uint8_t* data, uint8_t length);
bool writeEEPROMByte(uint8_t address, uint8_t memoryAddress, uint8_t data);
bool writeEEPROMBytes(uint8_t address, uint8_t memoryAddress, uint8_t* data, uint8_t length);
void dumpEEPROMContents(uint8_t address, uint8_t startAddr, uint8_t endAddr);
void testEEPROMReadWrite(uint8_t address);

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("=== AT24C02 EEPROM I2C Test ===");
    Serial.println(__FILE__);
    Serial.println();
    
    // Setup I2C
    setupI2C();
    
    // Scan for I2C devices
    Serial.println("Scanning I2C devices...");
    scanI2CDevices();
    Serial.println();
    
    // Test EEPROM
    if (detectEEPROM(EEPROM_I2C_ADDRESS)) {
        Serial.print("AT24C02 EEPROM detected at address 0x");
        Serial.println(EEPROM_I2C_ADDRESS, HEX);
        
        // Test read/write operations
        testEEPROMReadWrite(EEPROM_I2C_ADDRESS);
    } else {
        Serial.println("No AT24C02 EEPROM detected at address 0x50");
        Serial.println("Check your wiring and try scanning all I2C addresses.");
    }
}

void loop() {
    // This test runs once in setup()
    delay(1000);
}

void setupI2C() {
    Serial.println("Setting up I2C...");
    
    #if defined(SEEED_XIAO_RP2040)
        Wire1.setSDA(pin_i2c_sda);
        Wire1.setSCL(pin_i2c_scl);
        Wire1.setClock(400000);
        Wire1.begin();
        Serial.println("I2C initialized on Wire1 (RP2040)");
    #elif defined(SEEED_XIAO_RP2350)
        Wire1.setSDA(pin_i2c_sda);
        Wire1.setSCL(pin_i2c_scl);
        Wire1.setClock(1000000);
        Wire1.begin();
        Serial.println("I2C initialized on Wire1 (RP2350)");
    #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32C6)
        Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
        Serial.println("I2C initialized on Wire (ESP32C3/C6)");
    #elif defined(SEEED_XIAO_ESP32S3)
        Wire.begin(pin_i2c_sda, pin_i2c_scl, 400000);
        Serial.println("I2C initialized on Wire (ESP32S3)");
    #else
        Wire.begin();
        Serial.println("I2C initialized on default Wire");
    #endif
    
    Serial.println();
}

void scanI2CDevices() {
    Serial.println("I2C Device Scanner");
    Serial.println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
    
    for (int i = 0; i < 128; i += 16) {
        Serial.printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            int address = i + j;
            
            #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
                Wire1.beginTransmission(address);
                int error = Wire1.endTransmission();
            #else
                Wire.beginTransmission(address);
                int error = Wire.endTransmission();
            #endif
            
            if (error == 0) {
                Serial.printf("%02x ", address);
            } else if (error == 4) {
                Serial.print("XX ");
            } else {
                Serial.print("-- ");
            }
        }
        Serial.println();
    }
}

bool detectEEPROM(uint8_t address) {
    #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        Wire1.beginTransmission(address);
        int error = Wire1.endTransmission();
    #else
        Wire.beginTransmission(address);
        int error = Wire.endTransmission();
    #endif
    
    return (error == 0);
}

bool readEEPROMByte(uint8_t address, uint8_t memoryAddress, uint8_t& data) {
    #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        Wire1.beginTransmission(address);
        Wire1.write(memoryAddress);
        int error = Wire1.endTransmission(false);
        
        if (error != 0) return false;
        
        uint8_t bytesRead = Wire1.requestFrom((uint8_t)address, (uint8_t)1, (bool)true);
        if (bytesRead == 1 && Wire1.available()) {
            data = Wire1.read();
            return true;
        }
    #else
        Wire.beginTransmission(address);
        Wire.write(memoryAddress);
        int error = Wire.endTransmission(false);
        
        if (error != 0) return false;
        
        uint8_t bytesRead = Wire.requestFrom((uint8_t)address, (uint8_t)1, (bool)true);
        if (bytesRead == 1 && Wire.available()) {
            data = Wire.read();
            return true;
        }
    #endif
    
    return false;
}

bool readEEPROMBytes(uint8_t address, uint8_t memoryAddress, uint8_t* data, uint8_t length) {
    #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        Wire1.beginTransmission(address);
        Wire1.write(memoryAddress);
        int error = Wire1.endTransmission(false);
        
        if (error != 0) return false;
        
        uint8_t bytesRead = Wire1.requestFrom((uint8_t)address, (uint8_t)length, (bool)true);
        uint8_t actualBytesRead = 0;
        
        while (Wire1.available() && actualBytesRead < length) {
            data[actualBytesRead++] = Wire1.read();
        }
        
        return (actualBytesRead == length);
    #else
        Wire.beginTransmission(address);
        Wire.write(memoryAddress);
        int error = Wire.endTransmission(false);
        
        if (error != 0) return false;
        
        uint8_t bytesRead = Wire.requestFrom((uint8_t)address, (uint8_t)length, (bool)true);
        uint8_t actualBytesRead = 0;
        
        while (Wire.available() && actualBytesRead < length) {
            data[actualBytesRead++] = Wire.read();
        }
        
        return (actualBytesRead == length);
    #endif
}

bool writeEEPROMByte(uint8_t address, uint8_t memoryAddress, uint8_t data) {
    #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
        Wire1.beginTransmission(address);
        Wire1.write(memoryAddress);
        Wire1.write(data);
        int error = Wire1.endTransmission();
        
        if (error == 0) {
            delay(5); // AT24C02 write cycle time
            return true;
        }
    #else
        Wire.beginTransmission(address);
        Wire.write(memoryAddress);
        Wire.write(data);
        int error = Wire.endTransmission();
        
        if (error == 0) {
            delay(5); // AT24C02 write cycle time
            return true;
        }
    #endif
    
    return false;
}

bool writeEEPROMBytes(uint8_t address, uint8_t memoryAddress, uint8_t* data, uint8_t length) {
    uint8_t bytesWritten = 0;
    uint8_t currentAddr = memoryAddress;
    
    while (bytesWritten < length) {
        // Calculate how many bytes we can write in this page
        uint8_t pageOffset = currentAddr % EEPROM_PAGE_SIZE;
        uint8_t bytesInPage = EEPROM_PAGE_SIZE - pageOffset;
        uint8_t remainingBytes = length - bytesWritten;
        uint8_t bytesToWrite = min(bytesInPage, remainingBytes);
        
        #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
            Wire1.beginTransmission(address);
            Wire1.write(currentAddr);
            
            for (uint8_t i = 0; i < bytesToWrite; i++) {
                Wire1.write(data[bytesWritten + i]);
            }
            
            int error = Wire1.endTransmission();
        #else
            Wire.beginTransmission(address);
            Wire.write(currentAddr);
            
            for (uint8_t i = 0; i < bytesToWrite; i++) {
                Wire.write(data[bytesWritten + i]);
            }
            
            int error = Wire.endTransmission();
        #endif
        
        if (error != 0) {
            Serial.printf("Write error at address 0x%02X\n", currentAddr);
            return false;
        }
        
        bytesWritten += bytesToWrite;
        currentAddr += bytesToWrite;
        
        delay(5); // AT24C02 write cycle time
    }
    
    return true;
}

void dumpEEPROMContents(uint8_t address, uint8_t startAddr, uint8_t endAddr) {
    Serial.printf("EEPROM contents from 0x%02X to 0x%02X:\n", startAddr, endAddr);
    Serial.println("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    
    for (uint8_t addr = startAddr; addr <= endAddr; addr += 16) {
        Serial.printf("%02X: ", addr);
        
        for (uint8_t offset = 0; offset < 16; offset++) {
            uint8_t currentAddr = addr + offset;
            if (currentAddr <= endAddr) {
                uint8_t data;
                if (readEEPROMByte(address, currentAddr, data)) {
                    Serial.printf("%02X ", data);
                } else {
                    Serial.print("XX ");
                }
            } else {
                Serial.print("   ");
            }
        }
        
        // Print ASCII representation
        Serial.print(" |");
        for (uint8_t offset = 0; offset < 16; offset++) {
            uint8_t currentAddr = addr + offset;
            if (currentAddr <= endAddr) {
                uint8_t data;
                if (readEEPROMByte(address, currentAddr, data)) {
                    if (data >= 32 && data <= 126) {
                        Serial.write(data);
                    } else {
                        Serial.print(".");
                    }
                } else {
                    Serial.print("?");
                }
            }
        }
        Serial.println("|");
    }
}

void testEEPROMReadWrite(uint8_t address) {
    Serial.println("=== AT24C02 EEPROM Read/Write Test ===");
    
    // Show current state (don't clear)
    Serial.println("Current EEPROM contents (first 32 bytes):");
    dumpEEPROMContents(address, 0x00, 0x1F);
    Serial.println();
    
    // Test 1: Single byte operations
    Serial.println("=== Test 1: Single Byte Operations ===");
    uint8_t testByte = 0xAA;
    uint8_t testAddr = 0x00;
    
    Serial.printf("Writing 0x%02X to address 0x%02X...\n", testByte, testAddr);
    if (writeEEPROMByte(address, testAddr, testByte)) {
        Serial.println("Write successful!");
        delay(10);
        
        uint8_t readByte;
        if (readEEPROMByte(address, testAddr, readByte)) {
            Serial.printf("Read 0x%02X from address 0x%02X\n", readByte, testAddr);
            if (testByte == readByte) {
                Serial.println("✓ Single byte test passed!");
            } else {
                Serial.printf("✗ Single byte test failed! Expected: 0x%02X, Got: 0x%02X\n", testByte, readByte);
            }
        } else {
            Serial.println("✗ Single byte read failed!");
        }
    } else {
        Serial.println("✗ Single byte write failed!");
    }
    Serial.println();
    
    // Test 2: Sequential writes
    Serial.println("=== Test 2: Sequential Writes ===");
    uint8_t sequentialData[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t seqAddr = 0x10;
    uint8_t seqLength = sizeof(sequentialData);
    
    Serial.printf("Writing %d sequential bytes starting at 0x%02X...\n", seqLength, seqAddr);
    for (uint8_t i = 0; i < seqLength; i++) {
        if (writeEEPROMByte(address, seqAddr + i, sequentialData[i])) {
            Serial.printf("  Wrote 0x%02X to 0x%02X\n", sequentialData[i], seqAddr + i);
            delay(5);
        } else {
            Serial.printf("  Failed to write 0x%02X to 0x%02X\n", sequentialData[i], seqAddr + i);
        }
    }
    
    delay(20);
    
    Serial.println("Reading back sequential data...");
    for (uint8_t i = 0; i < seqLength; i++) {
        uint8_t readData;
        if (readEEPROMByte(address, seqAddr + i, readData)) {
            Serial.printf("  Read 0x%02X from 0x%02X %s\n", readData, seqAddr + i, 
                (readData == sequentialData[i]) ? "✓" : "✗");
        } else {
            Serial.printf("  Failed to read from 0x%02X\n", seqAddr + i);
        }
    }
    Serial.println();
    
    // Test 3: Page boundary crossing (8-byte pages for AT24C02)
    Serial.println("=== Test 3: Page Boundary Crossing ===");
    uint8_t pageData[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t pageAddr = 0x06; // Start near page boundary (8-byte pages)
    uint8_t pageLength = sizeof(pageData);
    
    Serial.printf("Writing %d bytes crossing page boundary at 0x%02X...\n", pageLength, pageAddr);
    if (writeEEPROMBytes(address, pageAddr, pageData, pageLength)) {
        Serial.println("Page boundary write successful!");
        delay(20);
        
        uint8_t readPageData[pageLength];
        if (readEEPROMBytes(address, pageAddr, readPageData, pageLength)) {
            Serial.println("Page boundary read successful!");
            bool pageMatch = true;
            for (uint8_t i = 0; i < pageLength; i++) {
                if (pageData[i] != readPageData[i]) {
                    pageMatch = false;
                    Serial.printf("  Mismatch at %d: Expected 0x%02X, Got 0x%02X\n", 
                        i, pageData[i], readPageData[i]);
                }
            }
            if (pageMatch) {
                Serial.println("✓ Page boundary test passed!");
            } else {
                Serial.println("✗ Page boundary test failed!");
            }
        } else {
            Serial.println("✗ Page boundary read failed!");
        }
    } else {
        Serial.println("✗ Page boundary write failed!");
    }
    Serial.println();
    
    // Test 4: Bulk operations
    Serial.println("=== Test 4: Bulk Operations ===");
    uint8_t testData[] = {0xAA, 0x55, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    uint8_t testAddr2 = 0x20;
    uint8_t testLength = sizeof(testData);
    
    Serial.printf("Writing %d bytes to address 0x%02X...\n", testLength, testAddr2);
    
    if (writeEEPROMBytes(address, testAddr2, testData, testLength)) {
        Serial.println("Bulk write successful!");
        delay(20);
        
        uint8_t readData[testLength];
        Serial.printf("Reading %d bytes from address 0x%02X...\n", testLength, testAddr2);
        
        if (readEEPROMBytes(address, testAddr2, readData, testLength)) {
            Serial.println("Bulk read successful!");
            
            bool dataMatch = true;
            Serial.println("Data comparison:");
            for (uint8_t i = 0; i < testLength; i++) {
                Serial.printf("  [%02X]: Write=0x%02X, Read=0x%02X %s\n", 
                    testAddr2 + i, testData[i], readData[i], 
                    (testData[i] == readData[i]) ? "✓" : "✗");
                if (testData[i] != readData[i]) {
                    dataMatch = false;
                }
            }
            
            if (dataMatch) {
                Serial.println("✓ All data matches!");
            } else {
                Serial.println("✗ Data mismatch detected!");
            }
        } else {
            Serial.println("✗ Bulk read failed!");
        }
    } else {
        Serial.println("✗ Bulk write failed!");
    }
    
    Serial.println();
    
    // Final dump
    Serial.println("=== Final EEPROM Contents ===");
    dumpEEPROMContents(address, 0x00, 0x3F);
    Serial.println();
    
    Serial.println("=== AT24C02 EEPROM Test Complete ===");
}
