#ifndef sysConfig_h
#define sysConfig_h

#include <Arduino.h>
#include "boardConfig.h"

#if defined(Proj_XIAOPowerBread)
#include "sysConfig_configData.h"
#include <EEPROM.h> //use the internal EEPROM of the XIAO board.
#elif defined(Proj_XIAOPowerMonitor)
#include "sysConfig_configData.h"
#include <SparkFun_External_EEPROM.h> //use the external EEPROM
#else
#error "Unknown project"  
#endif


// EEPROM address mapping
struct ConfigAddresses {
    static constexpr uint8_t CFG_VERSION = 0;
    static constexpr uint8_t DEFAULT_MODE = 1;
    static constexpr uint8_t DEFAULT_CHANNEL = 2;
    static constexpr uint8_t SHUNT_RESISTOR_CHA = 3;
    static constexpr uint8_t SHUNT_RESISTOR_CHB = 4;
    static constexpr uint8_t SERIAL_ENABLE = 5;
    static constexpr uint8_t SERIAL_BAUDRATE = 6;
    static constexpr uint8_t SERIAL_MODE = 7;
    static constexpr uint8_t SERIAL_PRINT_INTERVAL = 8;
    static constexpr uint8_t CHART_UPDATE_INTERVAL = 9;
    static constexpr uint8_t CHART_SCALE_MODE = 10;
    static constexpr uint8_t CHART_SCALE = 11;
};

// Configuration index enum for type safety
enum class ConfigIndex : uint8_t {
    DEFAULT_MODE = 0,
    DEFAULT_CHANNEL,
    SHUNT_RESISTOR_CHA,
    SHUNT_RESISTOR_CHB,
    SERIAL_ENABLE,
    SERIAL_BAUDRATE,
    SERIAL_MODE,
    SERIAL_PRINT_INTERVAL,
    CHART_UPDATE_INTERVAL,
    CHART_SCALE_MODE,
    CHART_SCALE,
    COUNT
};

// Configuration names for UI display
struct ConfigNames {
    static constexpr size_t MAX_NAME_LENGTH = 12;
    static constexpr const char* NAMES[] = {
        "def_mode",
        "def_ch",
        "shunt_chA",
        "shunt_chB",
        "uart_en",
        "uart_baud",
        "uart_mode",
        "uart_intv",
        "chart_intv",
        "chart_M",
        "chart_SC"
    };
    
    static const char* getName(ConfigIndex index) {
        return NAMES[static_cast<uint8_t>(index)];
    }
};



class SysConfig {
public:
    static constexpr uint16_t EEPROM_SIZE = 256; //Use 256 bytes both for the internal & external(24C02) EEPROM.
    
    // Constructor
    SysConfig();
    
    // Initialization
    bool begin();
    bool initialize(bool forceWrite = false);
    
    // Configuration access
    const ConfigData& getConfig() const { return cfgData_; }
    ConfigData& getConfig() { return cfgData_; }
    
    // Configuration modification
    bool setConfig(const ConfigData& newConfig);
    bool incrementValue(ConfigIndex index);
    bool decrementValue(ConfigIndex index);
    
    // EEPROM operations
    bool saveToEEPROM();
    bool loadFromEEPROM();
    bool resetToDefaults();
    
    // Utility methods
    String toString() const;
    bool isValid() const { return cfgData_.isValid(); }
    
    // Configuration limits
    static uint8_t getMinValue(ConfigIndex index);
    static uint8_t getMaxValue(ConfigIndex index);
    static bool isToggleType(ConfigIndex index);

private:
    #if defined(Proj_XIAOPowerBread)
    #elif defined(Proj_XIAOPowerMonitor)
    ExternalEEPROM extEEPROM;
    #endif

    ConfigData cfgData_;
    bool eepromInitialized_ = false;

    // Helper methods
    bool writeToEEPROM(uint8_t address, uint8_t value);
    uint8_t readFromEEPROM(uint8_t address);
    
    bool validateConfigValue(ConfigIndex index, uint8_t value);

    uint8_t getConfigValue(ConfigIndex index);
    bool setConfigValue(ConfigIndex index, uint8_t value);
};

#endif