#include "task_serial.h"
// #include "configMode.h"
// #include "sysConfig.h"

// External declarations
// extern ConfigMode configMode;
// extern SysConfig sysConfig;

// AT Command processing function
void processATCommand(String command) {
    command.trim(); // Remove leading/trailing whitespace
    
    // Convert to uppercase for case-insensitive comparison
    command.toUpperCase();
    
    // Basic AT command - just respond with OK
    if (command == "AT") {
        Serial.println("OK");
        return;
    }
    
    // AT+CONFIG command - enter config mode
    if (command == "AT+CONFIG") {
        Serial.println("Entering config mode...");
        // configMode.enterConfigMode();
        Serial.println("OK");
        return;
    }
    
    // AT+SET_Serial_Print_Interval command
    if (command.startsWith("AT+SET_SERIAL_PRINT_INTERVAL+")) {
        // Extract the value after the +
        String valueStr = command.substring(28); // Length of "AT+SET_SERIAL_PRINT_INTERVAL+"
        int value = valueStr.toInt();
        
        // Validate the value (0-4 as per sysConfig_user.h)
        if (value >= 0 && value <= 4) {
            // Update the configuration
            // sysConfig.cfg_data.serial_printInterval = value;
            
            // Save to EEPROM
            // sysConfig.saveConfig_to_EEPROM(sysConfig.cfg_data);
            
            Serial.print("Serial print interval set to: ");
            Serial.println(value);
            Serial.println("OK");
        } else {
            Serial.println("ERROR: Invalid value. Use 0-4 (0=1000ms, 1=500ms, 2=100ms, 3=50ms, 4=10ms)");
        }
        return;
    }
    
    // Unknown command
    Serial.println("ERROR: Unknown command");
}

// Function to read and process serial commands
void readSerialCommands() {
    static String commandBuffer = "";
    const int maxCommandLength = 128;
    
    while (Serial.available()) {
        char c = Serial.read();
        
        // Handle backspace
        if (c == '\b' || c == 127) {
            if (commandBuffer.length() > 0) {
                commandBuffer.remove(commandBuffer.length() - 1);
                Serial.print("\b \b"); // Clear the character on screen
            }
        }
        // Handle enter key (process command)
        else if (c == '\n' || c == '\r') {
            if (commandBuffer.length() > 0) {
                // Echo the received command
                Serial.print("Received command: ");
                Serial.println(commandBuffer);
                
                // Check if it's an AT command
                if (commandBuffer.startsWith("AT") || commandBuffer.startsWith("at")) {
                    processATCommand(commandBuffer);
                } else {
                    // Process other commands here (you can add command parsing logic later)
                    // For now, just acknowledge receipt
                    Serial.println("Command acknowledged");
                }
                
                // Clear the buffer
                commandBuffer = "";
            }
        }
        // Add character to buffer
        else if (commandBuffer.length() < maxCommandLength) {
            commandBuffer += c;
            Serial.print(c); // Echo the character
        }
    }
}

// Function to print sensor data
void printSensorData() {
    // Peek at the latest data from queue (non-blocking)
    SensorDataMessage receivedData;
    if (xQueuePeek(sensorDataQueue_serial, &receivedData, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Process the received data
        Serial.print("{\"cS\":[");
        for (size_t i = 0; i < sizeof(receivedData.data)/sizeof(receivedData.data[0]); i++) {
            if (i > 0) {
                Serial.print(",");
            }
            Serial.print("{\"ch\":");
            Serial.print(i);
            Serial.print(",\"v_mV\":");
            Serial.print(receivedData.data[i].busVoltage_mV);
            Serial.print(",\"c_mA\":");
            Serial.print(receivedData.data[i].current_mA);
            Serial.print(",\"p_mW\":");
            Serial.print(receivedData.data[i].power_mW);
            Serial.print(",\"ts\":");
            Serial.print(receivedData.timestamp);
            Serial.print("}");
        }
        Serial.println("]}");
    } else {
        // No data received within timeout
        Serial.println("{\"status\":\"no_sensor_data\"}");
    }
    //Example output: {"cS":[{"ch":0,"v_mV":1234,"c_mA":123,"p_mW":12345,"ts":1234567890},{"ch":1,"v_mV":1234,"c_mA":123,"p_mW":12345,"ts":1234567890}]}
}

void serialTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastPrintTime = 0;
    const TickType_t xPrintInterval = pdMS_TO_TICKS(1000); // Print every 1 second

    while (1)
    {
        TickType_t xCurrentTime = xTaskGetTickCount();
        
        // Read and process serial commands every 5ms
        readSerialCommands();
        
        // Print sensor data every 1 second
        if ((xCurrentTime - xLastPrintTime) >= xPrintInterval)
        {
            printSensorData();
            xLastPrintTime = xCurrentTime;
        }
        
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

        
