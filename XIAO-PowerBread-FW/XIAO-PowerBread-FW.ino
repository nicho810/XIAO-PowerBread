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

// Add this global variable near the top of the file, with other global declarations
volatile int dialStatus = 0; // 0: reset, 1: up, 2: down, 3: press
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;

//UIs
#include "ui_functions.h"
float old_chA_v = 0, old_chA_a = 0, old_chA_w = 0;
float old_chB_v = 0, old_chB_a = 0, old_chB_w = 0;
int tft_Rotation = 1;

//Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;

//ADC init
#define dial_adc A2
bool dial_enable = true;
volatile uint16_t dialValue = 0; // Variable to store ADC value

void dial_init() {
  pinMode(dial_adc, INPUT);
  analogReadResolution(12);  // Set the ADC resolution to 10 bits
}

int read_dial() {
  return analogRead(dial_adc);
}

#define STACK_SIZE 4096
StaticTask_t xTaskBuffer_UI;
StackType_t xStack_UI[STACK_SIZE];

#define STACK_SIZE_SERIAL 2048
StaticTask_t xTaskBuffer_Serial;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];

#define STACK_SIZE_DIAL 1024
StaticTask_t xTaskBuffer_Dial;
StackType_t xStack_Dial[STACK_SIZE_DIAL];

SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xMutexBuffer;

// Add this near the top of the file with other global declarations
TaskHandle_t xUITaskHandle = NULL;

void updateUITask(void *pvParameters) {
  (void) pvParameters;
  while (1) {
    if (rotationChangeRequested) {
      rotationChangeRequested = false;
      update_chAB_xy_by_Rotation(tft_Rotation);
      changeRotation(tft_Rotation, old_chA_v, old_chA_a, old_chA_w, old_chB_v, old_chB_a, old_chB_w);
      Serial.println("New rotation applied: " + String(tft_Rotation));
    }

    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    DualChannelData sensorData = inaSensor.readCurrentSensors();
    if (sensorData.channel0.isDirty) {
      ChannelInfoUpdate_A(
        sensorData.channel0.busVoltage,
        sensorData.channel0.busCurrent,
        sensorData.channel0.busPower,
        old_chA_v, old_chA_a, old_chA_w);
      old_chA_v = sensorData.channel0.busVoltage;
      old_chA_a = sensorData.channel0.busCurrent;
      old_chA_w = sensorData.channel0.busPower;
    }
    if (sensorData.channel1.isDirty) {
      ChannelInfoUpdate_B(
        sensorData.channel1.busVoltage,
        sensorData.channel1.busCurrent,
        sensorData.channel1.busPower,
        old_chB_v, old_chB_a, old_chB_w);
      old_chB_v = sensorData.channel1.busVoltage;
      old_chB_a = sensorData.channel1.busCurrent;
      old_chB_w = sensorData.channel1.busPower;
    }
    xSemaphoreGive(xSemaphore);
    // Serial.println("UI Task running");
    vTaskDelay(pdMS_TO_TICKS(50)); // Add a small delay
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
      Serial.printf("A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW | A2(12bit): %d\n",
                    sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000, sensorData.channel0.busCurrent, sensorData.channel0.busPower,
                    sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000, sensorData.channel1.busCurrent, sensorData.channel1.busPower,
                    dialValue);
      xSemaphoreGive(xSemaphore);
      xLastPrintTime = xCurrentTime;
      // Serial.println("Serial Task running");
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Increase delay to 100ms
  }
}

void dialReadTask(void *pvParameters) {
  (void) pvParameters;
  TickType_t xLastReadTime = 0;
  const TickType_t xReadInterval = pdMS_TO_TICKS(50); // Read every 50ms
  uint16_t lastDialValue = 0;
  int lastDialStatus = 0;
  while (1) {
    TickType_t xCurrentTime = xTaskGetTickCount();
    if ((xCurrentTime - xLastReadTime) >= xReadInterval) {
      dialValue = read_dial();
      if (dialValue != lastDialValue) {
        if (dialValue >= 300 && dialValue < 800) {
          dialStatus = 3;
          Serial.println("Dial pressed");
        } else if (dialValue >= 800 && dialValue < 1500) {
          dialStatus = 2;
          Serial.println("Dial turned down");
        } else if (dialValue >= 1500 && dialValue < 2000) {
          dialStatus = 1;
          Serial.println("Dial turned up");
        } else {
          dialStatus = 0; // Reset status for other values
          // Serial.println("Dial reset");
        }

        // Check if the dial was just released after being turned up or down
        if (dialStatus == 0 && (lastDialStatus == 1 || lastDialStatus == 2)) {
          int newRotation = tft_Rotation;
          if (lastDialStatus == 1) { // Was turned up
            newRotation++;
            if (newRotation > 3) newRotation = 0;
          } else if (lastDialStatus == 2) { // Was turned down
            newRotation--;
            if (newRotation < 0) newRotation = 3;
          }
          
          if (newRotation != tft_Rotation) {
            Serial.println("Requesting rotation change to " + String(newRotation));
            rotationChangeRequested = true;
            tft_Rotation = newRotation;
          } else {
            Serial.println("Rotation unchanged: " + String(tft_Rotation));
          }
        }

        lastDialStatus = dialStatus;
        lastDialValue = dialValue;
      }
      xLastReadTime = xCurrentTime;
    }
    vTaskDelay(pdMS_TO_TICKS(50)); // Increase delay to 50ms
  }
}

void setup(void) {
  // init serial
  Serial.begin(115200);
  Serial.println(F("Hello! XIAO PowerBread."));

  // LCD init
  tft.setSPISpeed(24000000);//50MHz
  tft.initR(INITR_GREENTAB); // Init ST7735S 0.96inch display (160*80), Also need to modify the _colstart = 24 and _rowstart = 0 in Adafruit_ST7735.cpp>initR(uint8_t)
  tft.setRotation(tft_Rotation); //Rotate the LCD 180 degree (0-3)
  update_chAB_xy_by_Rotation(tft_Rotation);

  tft.fillScreen(color_Background);

  //UI init
  drawUIFramework();

  //Current sensor init
  Wire.setClock(400000); // Set I2C to 400KHz
  Wire.begin();
  if (!inaSensor.begin()) {
    Serial.println("could not connect. Fix and Reboot");
  } else {
    Serial.println("INA3221 Found");
  }
  inaSensor.disableChannel(2);// Disable unused channel 2
  inaSensor.setShuntResistors(0.100, 0.100); // 100 mR shunt resistors for channels 0 and 1
  delay(100);

  //Dial init
  dial_init();

  xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
  if (xSemaphore == NULL) {
    Serial.println("Error creating semaphore");
    while(1);
  }

  // Adjust task priorities
  xUITaskHandle = xTaskCreateStatic(updateUITask, "UI_Update", STACK_SIZE, NULL, 3, xStack_UI, &xTaskBuffer_UI);
  TaskHandle_t xSerialHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);
  TaskHandle_t xDialHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);

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

// Add this function at the end of your file
extern "C" void vApplicationIdleHook(void)
{
  // This function will be called when the scheduler is idle
  Serial.println("Idle Hook called");
  while(1);
}

