#include "task_serial.h"


// AT Command processing function
void processATCommand(String command) {
    String response;
    if (atCommand.processCommand(command, response)) {
        Serial.println(response);
    } else {
        Serial.println("ERROR: Unknown command");
    }
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

        
