#include "ATCommand.h"


// Static command table
const ATCommand::ATCommandEntry ATCommand::commandTable[] = {
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

// Handler implementations
bool ATCommand::handleAT(String& cmd, String& resp) { resp = "[OK]"; return true; }
bool ATCommand::handleConfig(String& cmd, String& resp) { sysConfig.loadFromEEPROM(); resp = sysConfig.toString(); return true; }
bool ATCommand::handleReload(String& cmd, String& resp) { sysConfig.loadFromEEPROM(); resp = "OK"; return true; }
bool ATCommand::handleDefaultMode(String& cmd, String& resp) { String valueStr = cmd.substring(16); sysConfig.setConfigValue(ConfigIndex::DEFAULT_MODE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleDefaultChannel(String& cmd, String& resp) { String valueStr = cmd.substring(19); sysConfig.setConfigValue(ConfigIndex::DEFAULT_CHANNEL, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleShuntResistorCh1(String& cmd, String& resp) { String valueStr = cmd.substring(22); sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH1, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleShuntResistorCh2(String& cmd, String& resp) { String valueStr = cmd.substring(22); sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH2, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleShuntResistorCh3(String& cmd, String& resp) { String valueStr = cmd.substring(22); sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH3, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleShuntResistorCh4(String& cmd, String& resp) { String valueStr = cmd.substring(22); sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH4, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleSerialEnable(String& cmd, String& resp) { String valueStr = cmd.substring(17); sysConfig.setConfigValue(ConfigIndex::SERIAL_ENABLE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleSerialBaudrate(String& cmd, String& resp) { String valueStr = cmd.substring(19); sysConfig.setConfigValue(ConfigIndex::SERIAL_BAUDRATE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleSerialMode(String& cmd, String& resp) { String valueStr = cmd.substring(15); sysConfig.setConfigValue(ConfigIndex::SERIAL_MODE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleSerialPrintInterval(String& cmd, String& resp) { String valueStr = cmd.substring(25); sysConfig.setConfigValue(ConfigIndex::SERIAL_PRINT_INTERVAL, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleChartUpdateInterval(String& cmd, String& resp) { String valueStr = cmd.substring(25); sysConfig.setConfigValue(ConfigIndex::CHART_UPDATE_INTERVAL, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleChartScaleMode(String& cmd, String& resp) { String valueStr = cmd.substring(20); sysConfig.setConfigValue(ConfigIndex::CHART_SCALE_MODE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleChartScale(String& cmd, String& resp) { String valueStr = cmd.substring(15); sysConfig.setConfigValue(ConfigIndex::CHART_SCALE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleWifiEnable(String& cmd, String& resp) { String valueStr = cmd.substring(15); sysConfig.setConfigValue(ConfigIndex::WIFI_ENABLE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleWifiSsid(String& cmd, String& resp) { String valueStr = cmd.substring(13); sysConfig.setConfigValue(ConfigIndex::WIFI_SSID, valueStr); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleWifiPassword(String& cmd, String& resp) { String valueStr = cmd.substring(17); sysConfig.setConfigValue(ConfigIndex::WIFI_PASSWORD, valueStr); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleWebMode(String& cmd, String& resp) { String valueStr = cmd.substring(12); sysConfig.setConfigValue(ConfigIndex::WEB_MODE, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }
bool ATCommand::handleWebInterval(String& cmd, String& resp) { String valueStr = cmd.substring(16); sysConfig.setConfigValue(ConfigIndex::WEB_INTERVAL, valueStr.toInt()); sysConfig.saveToEEPROM(); resp = "OK:" + valueStr; return true; }


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
    response = "[ERROR] Unknown command";
    return false;
}


String ATCommand::response(String& response) {
    if (response == "[OK]") {
        return "[OK]"; //for AT only
    } else {
        return "[OK] " + response; //for other commands
    }
}

ATCommand::ATCommand() {
    // Constructor body (empty or add initialization if needed)
}
