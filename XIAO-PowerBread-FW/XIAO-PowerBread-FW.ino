//LCD init
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

//RTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

//LCD

#define TFT_CS -1  //CS is always connected to ground in this project.
#define TFT_RST D3
#define TFT_DC D9
#define TFT_MOSI D10
#define TFT_SCLK D8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float old_chA_v = 0, old_chA_a = 0, old_chA_w = 0;
float old_chB_v = 0, old_chB_a = 0, old_chB_w = 0;

// #define color_Background 0x0000
// #define color_Text 0xFFFF
// #define color_ChannelA 0x055B
// #define color_ChannelB 0xF97C
// uint8_t chA_x = 1;// UI global variables
// uint8_t chA_y = 82;// UI global variables
// uint8_t chB_x = 1;// UI global variables
// uint8_t chB_y = 0;// UI global variables

#include "ui_functions.h"

//Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;


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
    
    DualChannelData sensorData = inaSensor.readCurrentSensors();

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
      
      DualChannelData sensorData = inaSensor.readCurrentSensors();

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
  if (!inaSensor.begin()) {
    Serial.println("could not connect. Fix and Reboot");
  } else {
    Serial.println("INA3221 Found");
  }
  inaSensor.setShuntResistors(0.100, 0.100);
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
