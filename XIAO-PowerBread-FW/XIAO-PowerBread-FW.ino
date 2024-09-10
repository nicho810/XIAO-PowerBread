//LCD init
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

//rtos
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define TFT_CS -1  //CS is always connected to ground in this project.
#define TFT_RST D3
#define TFT_DC D9
#define TFT_MOSI D10
#define TFT_SCLK D8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define color_Background 0x0000
#define color_Text 0xFFFF
#define color_ChannelA 0x055B
#define color_ChannelB 0xF97C

//Current sensor init
#include "INA3221.h"
INA3221 INA(0x40);

struct INAData {
  float busVoltage;
  float current;
  float power;
  bool isDirty;  // Add this flag
};

struct DualChannelData {
  INAData channel0;
  INAData channel1;
};

float old_chA_v = 0, old_chA_a = 0, old_chA_w = 0;
float old_chB_v = 0, old_chB_a = 0, old_chB_w = 0;

//ADC init
#define dial_adc A2
bool dial_enable = true;
volatile uint16_t dialValue = 0;           // Variable to store ADC value

void dial_init() {
  pinMode(dial_adc, INPUT);
  analogReadResolution(12);  // Set the ADC resolution to 10 bits
}

int read_dial() {
  return analogRead(dial_adc);
}

// UI global variables
uint8_t chA_x = 1;
uint8_t chA_y = 82;
uint8_t chB_x = 1;
uint8_t chB_y = 0;

DualChannelData readCurrentSensors() {
  static DualChannelData prevINAData;
  DualChannelData INAData;

  // Read channel 0
  INAData.channel0.busVoltage = INA.getBusVoltage(0);
  INAData.channel0.current = INA.getCurrent_mA(0);
  INAData.channel0.power = INA.getPower_mW(0);

  // Read channel 1
  INAData.channel1.busVoltage = INA.getBusVoltage(1);
  INAData.channel1.current = INA.getCurrent_mA(1);
  INAData.channel1.power = INA.getPower_mW(1);

  // Check if values have changed
  INAData.channel0.isDirty = (INAData.channel0.busVoltage != prevINAData.channel0.busVoltage) || (INAData.channel0.current != prevINAData.channel0.current) || (INAData.channel0.power != prevINAData.channel0.power);

  INAData.channel1.isDirty = (INAData.channel1.busVoltage != prevINAData.channel1.busVoltage) || (INAData.channel1.current != prevINAData.channel1.current) || (INAData.channel1.power != prevINAData.channel1.power);

  prevINAData = INAData;
  return INAData;
}


void ChannelInfoUpdate_A(float new_chA_v, float new_chA_a, float new_chA_w, float old_chA_v, float old_chA_a, float old_chA_w, uint16_t color = color_Text) {
  uint8_t chA_1_x = chA_x + 4;
  uint8_t chA_1_y = chA_y + 30;
  uint8_t chA_2_y = chA_y + 50;
  uint8_t chA_3_y = chA_y + 70;
  uint8_t chA_v_x = chA_x + 70;
  uint8_t chA_mA_x = chA_x + 64;
  uint8_t chA_w_x = chA_x + 64;

  int offset_defaultFont = -6;

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Update voltage if changed
  if (new_chA_v != old_chA_v) {
    updateChangedDigits(chA_1_x, chA_1_y, old_chA_v, new_chA_v, color);
  }

  // Update current if changed
  if (new_chA_a != old_chA_a) {
    updateChangedDigits(chA_1_x, chA_2_y, old_chA_a, new_chA_a, color);
  }

  // Update power if changed
  if (new_chA_w != old_chA_w) {
    updateChangedDigits(chA_1_x, chA_3_y, old_chA_w, new_chA_w, color);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chA_v_x, chA_1_y + offset_defaultFont);
  tft.println("V");
  tft.setCursor(chA_mA_x, chA_2_y + offset_defaultFont);
  tft.println("mA");
  tft.setCursor(chA_w_x, chA_3_y + offset_defaultFont);
  tft.println("mW");
}

void ChannelInfoUpdate_B(float new_chB_v, float new_chB_a, float new_chB_w, float old_chB_v, float old_chB_a, float old_chB_w, uint16_t color = color_Text) {
  uint8_t chB_1_x = chB_x + 4;
  uint8_t chB_1_y = chB_y + 30;
  uint8_t chB_2_y = chB_y + 50;
  uint8_t chB_3_y = chB_y + 70;
  uint8_t chB_v_x = chB_x + 70;
  uint8_t chB_mA_x = chB_x + 64;
  uint8_t chB_w_x = chB_x + 64;

  int offset_defaultFont = -6;

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Update voltage if changed
  if (new_chB_v != old_chB_v) {
    updateChangedDigits(chB_1_x, chB_1_y, old_chB_v, new_chB_v, color);
  }

  // Update current if changed
  if (new_chB_a != old_chB_a) {
    updateChangedDigits(chB_1_x, chB_2_y, old_chB_a, new_chB_a, color);
  }

  // Update power if changed
  if (new_chB_w != old_chB_w) {
    updateChangedDigits(chB_1_x, chB_3_y, old_chB_w, new_chB_w, color);
  }

  // Print units (these don't need to be erased or updated)
  tft.setFont();
  tft.setTextColor(color);
  tft.setCursor(chB_v_x, chB_1_y + offset_defaultFont);
  tft.println("V");
  tft.setCursor(chB_mA_x, chB_2_y + offset_defaultFont);
  tft.println("mA");
  tft.setCursor(chB_w_x, chB_3_y + offset_defaultFont);
  tft.println("mW");
}

void updateChangedDigits(int x, int y, float oldValue, float newValue, uint16_t color) {
  char oldStr[10], newStr[10];
  char format[10];
  
  // Determine the number of decimal places automatically
  int decimalPlaces = 3;  // Default to 3 decimal places
  if (oldValue >= 10 || newValue >= 10) {
    decimalPlaces = 2;
  } else if (oldValue >= 100 || newValue >= 100) {
    decimalPlaces = 1;
  } else if (oldValue >= 1000 || newValue >= 1000) {
    decimalPlaces = 0;
  }
  
  snprintf(format, sizeof(format), "%%5.%df", decimalPlaces);
  snprintf(oldStr, sizeof(oldStr), format, oldValue);
  snprintf(newStr, sizeof(newStr), format, newValue);

  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);

  // Get bounds of the entire string
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(oldStr, x, y, &x1, &y1, &w, &h);

  // Erase the entire old string
  tft.fillRect(x1, y1, w, h, color_Background);

  // Draw the new string
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(newStr);
}

void drawUIFramework() {
  tft.drawRoundRect(chA_x, chA_y, 78, 78, 4, color_ChannelA);
  tft.fillRoundRect(chA_x, chA_y, 65, 12, 4, color_ChannelA);

  tft.drawRoundRect(chB_x, chB_y, 78, 78, 4, color_ChannelB);
  tft.fillRoundRect(chB_x, chB_y, 65, 12, 4, color_ChannelB);

  tft.setFont();
  tft.setTextColor(color_Text);
  tft.setTextSize(0);
  tft.setCursor(chA_x + 5, chA_y + 2);
  tft.println("Channel A");
  tft.setCursor(chB_x + 5, chB_y + 2);
  tft.println("Channel B");
}



#define STACK_SIZE 2048
StaticTask_t xTaskBuffer_UI;
StackType_t xStack_UI[STACK_SIZE];

#define STACK_SIZE_SERIAL 1024
StaticTask_t xTaskBuffer_Serial;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];

SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xMutexBuffer;

void updateUITask(void *pvParameters) {
  (void) pvParameters;
  while (1) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    
    DualChannelData sensorData = readCurrentSensors();

    if (sensorData.channel0.isDirty) {
      ChannelInfoUpdate_A(
        sensorData.channel0.busVoltage,
        sensorData.channel0.current,
        sensorData.channel0.power,
        old_chA_v, old_chA_a, old_chA_w);
      old_chA_v = sensorData.channel0.busVoltage;
      old_chA_a = sensorData.channel0.current;
      old_chA_w = sensorData.channel0.power;
    }

    if (sensorData.channel1.isDirty) {
      ChannelInfoUpdate_B(
        sensorData.channel1.busVoltage,
        sensorData.channel1.current,
        sensorData.channel1.power,
        old_chB_v, old_chB_a, old_chB_w);
      old_chB_v = sensorData.channel1.busVoltage;
      old_chB_a = sensorData.channel1.current;
      old_chB_w = sensorData.channel1.power;
    }

    xSemaphoreGive(xSemaphore);
    vTaskDelay(pdMS_TO_TICKS(100)); // Update UI every 100ms
  }
}

void serialPrintTask(void *pvParameters) {
  (void) pvParameters;
  TickType_t xLastPrintTime = 0;
  const TickType_t xPrintInterval = pdMS_TO_TICKS(1000);

  while (1) {
    TickType_t xCurrentTime = xTaskGetTickCount();
    
    if ((xCurrentTime - xLastPrintTime) >= xPrintInterval) {
      xSemaphoreTake(xSemaphore, portMAX_DELAY);
      
      DualChannelData sensorData = readCurrentSensors();

      Serial.printf("A: %.2fV %.2fmA %.2fmW | B: %.2fV %.2fmA %.2fmW\n",
                    sensorData.channel0.busVoltage, sensorData.channel0.current, sensorData.channel0.power,
                    sensorData.channel1.busVoltage, sensorData.channel1.current, sensorData.channel1.power);

      xSemaphoreGive(xSemaphore);
      
      xLastPrintTime = xCurrentTime;
    }
    
    vTaskDelay(pdMS_TO_TICKS(10)); // Short delay to prevent tight looping
  }
}

void setup(void) {
  delay(500);
  // init serial
  Serial.begin(115200);
  Serial.println(F("Hello! XIAO PowerBread."));

  // LCD init
  tft.initR(INITR_GREENTAB);
  tft.setRotation(2);
  tft.setSPISpeed(50000000);
  tft.fillScreen(color_Background);

  //UI init
  drawUIFramework();

  //Current sensor init
  Wire.begin();
  if (!INA.begin()) {
    Serial.println("could not connect. Fix and Reboot");
  } else {
    Serial.println("INA3221 Found");
  }
  INA.setShuntR(0, 0.100);
  INA.setShuntR(1, 0.100);
  Wire.setClock(400000);
  delay(100);

  //Dial
  dial_init();

  xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
  if (xSemaphore == NULL) {
    Serial.println("Error creating semaphore");
    while(1);
  }

  TaskHandle_t xHandle = xTaskCreateStatic(updateUITask, "UI_Update", STACK_SIZE, NULL, 1, xStack_UI, &xTaskBuffer_UI);
  if (xHandle == NULL) {
    Serial.println("Error creating UI task");
    while(1);
  }

  TaskHandle_t xSerialHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);
  if (xSerialHandle == NULL) {
    Serial.println("Error creating Serial Print task");
    while(1);
  }

  Serial.println("Starting scheduler");
  vTaskStartScheduler();

  // We should never get here as control is now taken by the scheduler
  Serial.println("Scheduler failed to start");
  while(1);
}

void loop() {
  // Empty, as tasks will handle the main functionality
}

// Add this function at the end of your file
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  Serial.print("Stack Overflow in task: ");
  Serial.println(pcTaskName);
  while(1);
}
