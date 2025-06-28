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

    // AT+SET_Serial_Print_Interval command
    if (cmd.startsWith("AT+SET_SERIAL_PRINT_INTERVAL+")) {
        // Extract the value after the +
        String valueStr = cmd.substring(28); // Length of "AT+SET_SERIAL_PRINT_INTERVAL+"
        int value = valueStr.toInt();
        
        // Validate the value (0-4 as per sysConfig_configData.h)
        if (value >= 0 && value <= 4) {
            // Update the configuration
            sysConfig.setConfigValue(ConfigIndex::SERIAL_PRINT_INTERVAL, value);
            sysConfig.saveToEEPROM();
            
            response = "> Serial print interval set to: " + String(value);
        } else {
            response = "> ERROR: Invalid value. Use 0-4 (0=1000ms, 1=500ms, 2=100ms, 3=50ms, 4=10ms)";
        }
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
