// Test command: pio test  --environment seeed_xiao_rp2040 --filter test_WebSerial -vv

#include <Arduino.h>
#include <ArduinoJson.h>

// Configuration
const unsigned long SEND_INTERVAL = 100; // Send data every 100ms
const float SINE_FREQUENCY = 0.5; // 0.5 Hz sine wave
const float BASE_CURRENT = 100.0; // Base current in mA
const float CURRENT_AMPLITUDE = 50.0; // Current amplitude in mA
const float BASE_VOLTAGE = 5000.0; // Base voltage in mV
const float VOLTAGE_AMPLITUDE = 500.0; // Voltage amplitude in mV

// Global variables
unsigned long lastSendTime = 0;
unsigned long startTime = 0;
int channelCounter = 0;

// Function to generate sine wave value
float generateSineWave(float amplitude, float frequency, float offset, unsigned long timeMs) {
    float angle = 2 * PI * frequency * (timeMs / 1000.0);
    return amplitude * sin(angle) + offset;
}

// Function to calculate power from voltage and current
float calculatePower(float voltage_mV, float current_mA) {
    return (voltage_mV * current_mA) / 1000.0; // Convert to mW
}

// Function to send JSON data
void sendCurrentSensorData() {
    unsigned long currentTime = millis();
    unsigned long timestamp = currentTime + startTime;
    
    // Create JSON document
    StaticJsonDocument<1024> doc;
    JsonArray cSArray = doc.createNestedArray("cS");
    
    // Generate data for multiple channels (0-3)
    for (int ch = 0; ch < 4; ch++) {
        JsonObject channel = cSArray.createNestedObject();
        
        // Generate sine wave values with different phases for each channel
        float phaseOffset = ch * (PI / 2); // 90-degree phase difference between channels
        float timeWithPhase = (currentTime / 1000.0) + phaseOffset;
        
        // Generate voltage (sine wave)
        float voltage = generateSineWave(VOLTAGE_AMPLITUDE, SINE_FREQUENCY, BASE_VOLTAGE, currentTime);
        voltage += (ch * 100); // Add offset for different channels
        
        // Generate current (sine wave with different phase)
        float current = generateSineWave(CURRENT_AMPLITUDE, SINE_FREQUENCY, BASE_CURRENT, currentTime);
        current += (ch * 20); // Add offset for different channels
        
        // Calculate power
        float power = calculatePower(voltage, current);
        
        // Fill JSON object
        channel["ch"] = ch;
        channel["v_mV"] = round(voltage);
        channel["c_mA"] = round(current * 10) / 10.0; // Round to 1 decimal place
        channel["p_mW"] = round(power);
        channel["ts"] = timestamp;
    }
    
    // Serialize JSON to string
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Send via Serial
    Serial.println(jsonString);
    
    // Optional: Send via Serial1 if available (for debugging)
    #ifdef Serial1
    Serial1.println(jsonString);
    #endif
}

void setup() {
    // Initialize Serial communication
    Serial.begin(115200);
    
    #ifdef Serial1
    Serial1.begin(115200);
    #endif
    
    // Wait for serial connection
    while (!Serial) {
        delay(10);
    }
    
    // Initialize start time
    startTime = millis();
    
    Serial.println("Current Sensor Sine Wave Demo Started");
    Serial.println("Sending JSON data every 100ms...");
    Serial.println("Format: {\"cS\":[{\"ch\":0,\"v_mV\":1234,\"c_mA\":123,\"p_mW\":12345,\"ts\":1234567890}, ...]}");
    Serial.println("----------------------------------------");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Send data at specified interval
    if (currentTime - lastSendTime >= SEND_INTERVAL) {
        sendCurrentSensorData();
        lastSendTime = currentTime;
    }
    
    // Handle incoming serial commands
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "help") {
            Serial.println("Available commands:");
            Serial.println("  help - Show this help");
            Serial.println("  status - Show current status");
            Serial.println("  stop - Stop sending data");
            Serial.println("  start - Start sending data");
        }
        else if (command == "status") {
            Serial.print("Running time: ");
            Serial.print((currentTime - startTime) / 1000.0);
            Serial.println(" seconds");
            Serial.print("Channel count: 4");
            Serial.println();
        }
        else if (command == "stop") {
            Serial.println("Data transmission stopped. Send 'start' to resume.");
            // You could add a flag here to stop sending data
        }
        else if (command == "start") {
            Serial.println("Data transmission resumed.");
        }
    }
    
    // Small delay to prevent overwhelming the system
    delay(1);
}
