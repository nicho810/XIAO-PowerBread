#include "ATCommand.h"

ATCommand::ATCommand() {
}

ATCommand::~ATCommand() {
}


bool ATCommand::processCommand(String& command, String& response) {
    String cmd = command;
    cmd.trim(); // Remove leading/trailing whitespace
    cmd.toUpperCase();

    // AT: respond with OK
    if (cmd == "AT") {
        response = "OK";
        return true;
    }

    // AT+CONFIG: print config data
    if (cmd == "AT+CONFIG") {
        sysConfig.loadFromEEPROM();
        response = sysConfig.toString() ;
        return true;
    }

    // AT+RELOAD: reload config data from EEPROM
    if (cmd == "AT+RELOAD") {
        sysConfig.loadFromEEPROM();
        response = "OK";
        return true;
    }

    // AT+DEFAULT_MODE: set default mode
    if (cmd.startsWith("AT+DEFAULT_MODE+")) {
        String valueStr = cmd.substring(16); // Length of "AT+DEFAULT_MODE+"
        sysConfig.setConfigValue(ConfigIndex::DEFAULT_MODE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+DEFAULT_CHANNEL: set default channel
    if (cmd.startsWith("AT+DEFAULT_CHANNEL+")) {
        String valueStr = cmd.substring(19); // Length of "AT+DEFAULT_CHANNEL+"
        sysConfig.setConfigValue(ConfigIndex::DEFAULT_CHANNEL, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SHUNT_RESISTOR_CH1: set shunt resistor for channel 1
    if (cmd.startsWith("AT+SHUNT_RESISTOR_CH1+")) {
        String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH1+"
        sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH1, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SHUNT_RESISTOR_CH2: set shunt resistor for channel 2
    if (cmd.startsWith("AT+SHUNT_RESISTOR_CH2+")) {
        String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH2+"
        sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH2, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SHUNT_RESISTOR_CH3: set shunt resistor for channel 3  
    if (cmd.startsWith("AT+SHUNT_RESISTOR_CH3+")) {
        String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH3+"
        sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH3, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
    }       

    // AT+SHUNT_RESISTOR_CH4: set shunt resistor for channel 4
    if (cmd.startsWith("AT+SHUNT_RESISTOR_CH4+")) {
        String valueStr = cmd.substring(22); // Length of "AT+SHUNT_RESISTOR_CH4+"
        sysConfig.setConfigValue(ConfigIndex::SHUNT_RESISTOR_CH4, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SERIAL_ENABLE: set serial enable
    if (cmd.startsWith("AT+SERIAL_ENABLE+")) {
        String valueStr = cmd.substring(17); // Length of "AT+SERIAL_ENABLE+"
        sysConfig.setConfigValue(ConfigIndex::SERIAL_ENABLE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SERIAL_BAUDRATE: set serial baudrate
    if (cmd.startsWith("AT+SERIAL_BAUDRATE+")) {
        String valueStr = cmd.substring(19); // Length of "AT+SERIAL_BAUDRATE+"
        sysConfig.setConfigValue(ConfigIndex::SERIAL_BAUDRATE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SERIAL_MODE: set serial mode
    if (cmd.startsWith("AT+SERIAL_MODE+")) {
        String valueStr = cmd.substring(15); // Length of "AT+SERIAL_MODE+"
        sysConfig.setConfigValue(ConfigIndex::SERIAL_MODE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+SERIAL_PRINT_INTERVAL: set serial print interval
    if (cmd.startsWith("AT+SERIAL_PRINT_INTERVAL+")) {
        String valueStr = cmd.substring(25); // Length of "AT+SERIAL_PRINT_INTERVAL+"
        sysConfig.setConfigValue(ConfigIndex::SERIAL_PRINT_INTERVAL, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+CHART_UPDATE_INTERVAL: set chart update interval
    if (cmd.startsWith("AT+CHART_UPDATE_INTERVAL+")) {
        String valueStr = cmd.substring(25); // Length of "AT+CHART_UPDATE_INTERVAL+"
        sysConfig.setConfigValue(ConfigIndex::CHART_UPDATE_INTERVAL, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+CHART_SCALE_MODE: set chart scale mode
    if (cmd.startsWith("AT+CHART_SCALE_MODE+")) {
        String valueStr = cmd.substring(20); // Length of "AT+CHART_SCALE_MODE+"
        sysConfig.setConfigValue(ConfigIndex::CHART_SCALE_MODE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+CHART_SCALE: set chart scale
    if (cmd.startsWith("AT+CHART_SCALE+")) {
        String valueStr = cmd.substring(15); // Length of "AT+CHART_SCALE+"
        sysConfig.setConfigValue(ConfigIndex::CHART_SCALE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }


    // AT+WIFI_ENABLE
    if (cmd.startsWith("AT+WIFI_ENABLE+")) {
        String valueStr = cmd.substring(15); // Length of "AT+WIFI_ENABLE+"
        sysConfig.setConfigValue(ConfigIndex::WIFI_ENABLE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+WIFI_SSID
    if (cmd.startsWith("AT+WIFI_SSID+")) {
        String valueStr = cmd.substring(13); // Length of "AT+WIFI_SSID+"
        sysConfig.setConfigValue(ConfigIndex::WIFI_SSID, valueStr);
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    // AT+WIFI_PASSWORD
    if (cmd.startsWith("AT+WIFI_PASSWORD+")) {
        String valueStr = cmd.substring(17); // Length of "AT+WIFI_PASSWORD+"
        sysConfig.setConfigValue(ConfigIndex::WIFI_PASSWORD, valueStr);
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    //WEB_MODE
    if (cmd.startsWith("AT+WEB_MODE+")) {
        String valueStr = cmd.substring(12); // Length of "AT+WEB_MODE+"
        sysConfig.setConfigValue(ConfigIndex::WEB_MODE, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }

    //WEB_INTERVAL
    if (cmd.startsWith("AT+WEB_INTERVAL+")) {
        String valueStr = cmd.substring(16); // Length of "AT+WEB_INTERVAL+"
        sysConfig.setConfigValue(ConfigIndex::WEB_INTERVAL, valueStr.toInt());
        sysConfig.saveToEEPROM();
        response = "OK:" + valueStr;
        return true;
    }


    // Unknown command
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
