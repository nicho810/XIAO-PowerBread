#include "ATCommand.h"

ATCommand::ATCommand() {
}

ATCommand::~ATCommand() {
}

typedef bool (*ATHandler)(String&, String&);

struct ATCommandEntry {
    const char* command;
    ATHandler handler;
    int argStartIndex; // where the argument starts in the string, if needed
};

static bool handleAT(String& cmd, String& resp) {
    resp = "OK";
    return true;
}

static bool handleConfig(String& cmd, String& resp) {
    sysConfig.loadFromEEPROM();
    resp = sysConfig.toString();
    return true;
}

static bool handleReload(String& cmd, String& resp) {
    sysConfig.loadFromEEPROM();
    resp = "OK";
    return true;
}

static bool handleDefaultMode(String& cmd, String& resp) {
    String valueStr = cmd.substring(16); // Length of "AT+DEFAULT_MODE+"
    sysConfig.setConfigValue(ConfigIndex::DEFAULT_MODE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleDefaultChannel(String& cmd, String& resp) {
    String valueStr = cmd.substring(19); // Length of "AT+DEFAULT_CHANNEL+"
    sysConfig.setConfigValue(ConfigIndex::DEFAULT_CHANNEL, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleShuntResistorCh1(String& cmd, String& resp) {
    String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH1+"
    sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH1, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleShuntResistorCh2(String& cmd, String& resp) {
    String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH2+"
    sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH2, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleShuntResistorCh3(String& cmd, String& resp) {
    String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH3+"
    sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH3, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleShuntResistorCh4(String& cmd, String& resp) {
    String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH4+"
    sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH4, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleSerialEnable(String& cmd, String& resp) {
    String valueStr = cmd.substring(17); // Length of "AT+SERIAL_ENABLE+"
    sysConfig.setConfigValue(ConfigIndex::SERIAL_ENABLE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleSerialBaudrate(String& cmd, String& resp) {
    String valueStr = cmd.substring(19); // Length of "AT+SERIAL_BAUDRATE+"
    sysConfig.setConfigValue(ConfigIndex::SERIAL_BAUDRATE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleSerialMode(String& cmd, String& resp) {
    String valueStr = cmd.substring(15); // Length of "AT+SERIAL_MODE+"
    sysConfig.setConfigValue(ConfigIndex::SERIAL_MODE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleSerialPrintInterval(String& cmd, String& resp) {
    String valueStr = cmd.substring(25); // Length of "AT+SERIAL_PRINT_INTERVAL+"
    sysConfig.setConfigValue(ConfigIndex::SERIAL_PRINT_INTERVAL, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleChartUpdateInterval(String& cmd, String& resp) {
    String valueStr = cmd.substring(25); // Length of "AT+CHART_UPDATE_INTERVAL+"
    sysConfig.setConfigValue(ConfigIndex::CHART_UPDATE_INTERVAL, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleChartScaleMode(String& cmd, String& resp) {
    String valueStr = cmd.substring(20); // Length of "AT+CHART_SCALE_MODE+"
    sysConfig.setConfigValue(ConfigIndex::CHART_SCALE_MODE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleChartScale(String& cmd, String& resp) {
    String valueStr = cmd.substring(15); // Length of "AT+CHART_SCALE+"
    sysConfig.setConfigValue(ConfigIndex::CHART_SCALE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleWifiEnable(String& cmd, String& resp) {
    String valueStr = cmd.substring(15); // Length of "AT+WIFI_ENABLE+"
    sysConfig.setConfigValue(ConfigIndex::WIFI_ENABLE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleWifiSsid(String& cmd, String& resp) {
    String valueStr = cmd.substring(13); // Length of "AT+WIFI_SSID+"
    sysConfig.setConfigValue(ConfigIndex::WIFI_SSID, valueStr);
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleWifiPassword(String& cmd, String& resp) {
    String valueStr = cmd.substring(17); // Length of "AT+WIFI_PASSWORD+"
    sysConfig.setConfigValue(ConfigIndex::WIFI_PASSWORD, valueStr);
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleWebMode(String& cmd, String& resp) {
    String valueStr = cmd.substring(12); // Length of "AT+WEB_MODE+"
    sysConfig.setConfigValue(ConfigIndex::WEB_MODE, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

static bool handleWebInterval(String& cmd, String& resp) {
    String valueStr = cmd.substring(16); // Length of "AT+WEB_INTERVAL+"
    sysConfig.setConfigValue(ConfigIndex::WEB_INTERVAL, valueStr.toInt());
    sysConfig.saveToEEPROM();
    resp = "OK:" + valueStr;
    return true;
}

const ATCommandEntry commandTable[] = {
    {"AT", handleAT, -1},
    {"AT+CONFIG", handleConfig, -1},
    {"AT+RELOAD", handleReload, -1},
    {"AT+DEFAULT_MODE+", handleDefaultMode, 16},
    {"AT+DEFAULT_CHANNEL+", handleDefaultChannel, 19},
    {"AT+SHUNT_RESISTOR_CH1+", handleShuntResistorCh1, 22},
    {"AT+SHUNT_RESISTOR_CH2+", handleShuntResistorCh2, 22},
    {"AT+SHUNT_RESISTOR_CH3+", handleShuntResistorCh3, 22},
    {"AT+SHUNT_RESISTOR_CH4+", handleShuntResistorCh4, 22},
    {"AT+SERIAL_ENABLE+", handleSerialEnable, 17},
    {"AT+SERIAL_BAUDRATE+", handleSerialBaudrate, 19},
    {"AT+SERIAL_MODE+", handleSerialMode, 15},
    {"AT+SERIAL_PRINT_INTERVAL+", handleSerialPrintInterval, 25},
    {"AT+CHART_UPDATE_INTERVAL+", handleChartUpdateInterval, 25},
    {"AT+CHART_SCALE_MODE+", handleChartScaleMode, 20},
    {"AT+CHART_SCALE+", handleChartScale, 15},
    {"AT+WIFI_ENABLE+", handleWifiEnable, 15},
    {"AT+WIFI_SSID+", handleWifiSsid, 13},
    {"AT+WIFI_PASSWORD+", handleWifiPassword, 17},
    {"AT+WEB_MODE+", handleWebMode, 12},
    {"AT+WEB_INTERVAL+", handleWebInterval, 16},
};

bool ATCommand::processCommand(String& command, String& response) {
    String cmd = command;
    cmd.trim();
    cmd.toUpperCase();

    for (const auto& entry : commandTable) {
        if (cmd == entry.command) {
            return entry.handler(cmd, response);
        }
        // For commands with arguments, use startsWith and substring as needed
    }
    response = "ERROR: Unknown command";
    return false;
}



bool ATCommand::sendError(String& error) {
    Serial.println(error);
    return true;
}

bool ATCommand::sendOK(String& response) {
    Serial.println(response);
    return true;
}
