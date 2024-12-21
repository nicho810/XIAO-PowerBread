/*
 * XIAO PowerBread - A Breadboard Power Supply with Real-Time Monitoring
 * 
 * This open-source project provides a reliable and efficient power solution 
 * for breadboard prototyping, featuring built-in sensors, real-time monitoring.
 * 
 * Key Features:
 * - Real-Time Monitoring
 * - High-Current Output (up to 1.5A of 3.3V power)
 * - Built-in LCD Display
 * - Plug-and-Play Design
 * - Open-Source and Seeed XIAO Powered
 * - Dual-Channel Voltage and Current Sensing
 * - Compact Design with 3.3V and 5V outputs
 * - Multiple UI functions
 * 
 * For more information, visit:
 * https://github.com/nicho810/XIAO-PowerBread
 * 
 * This project uses the following libraries:
 * - INA3221_RT Library: https://github.com/RobTillaart/INA3221_RT/tree/master
 * - Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
 * - Adafruit ST7735 Library: https://github.com/adafruit/Adafruit-ST7735-Library
 * - Arduino-Pico Core: https://github.com/earlephilhower/arduino-pico
 * - Arduino-ESP32: https://github.com/espressif/arduino-esp32
 * - Adafruit SleepyDog Library: https://github.com/adafruit/Adafruit_SleepyDog
 * Note: A modified version of Adafruit_ST7735 library is included since v1.1.2 to fit the LCD module used in this project.
 * We are grateful to the developers and contributors of these libraries.
 * 
 * Licensed under the MIT License
 */

//=================================================================
// IMPORTANT: Make sure to define the board type in boardConfig.h !!!
//=================================================================
#include "boardConfig.h"

//LCD init
#include <Adafruit_GFX.h>     // Core graphics library
#include "src/tft_driver/XPB_ST7735.h"  // Hardware-specific library for ST7735, modified from Adafruit_ST7735
// #include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

//RTOS
#if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#elif defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C6)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#endif

// Watchdog include
#if defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C6)
    // No watchdog library needed for ESP32-based boards (S3, C3, C6)
#else
    #include <Adafruit_SleepyDog.h>
#endif

//LCD
#define TFT_CS -1  //CS is always connected to ground in this project.
#define TFT_RST D3
#define TFT_DC D9
#define TFT_MOSI D10
#define TFT_SCLK D8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//LED
#define LED_RGB_R 17
#define LED_RGB_G 16
#define LED_RGB_B 25

//Dial Switch
#include "src/dialSwitch.h"

DialFunction dial;
volatile int dialStatus = 0;  // 0: reset, 1: up, 2: down, 3: press, 4: long press
volatile int lastDialStatus = 0;

// LCD Rotation
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 2;  // default rotation.

//UIs
#include "src/ui/systemUI_functions.h"
#include "src/ui/dataMonitor_functions.h"
#include "src/ui/dataMonitorChart_functions.h"
#include "src/ui/dataMonitorCount_functions.h"
#include "src/ui/dataChart_functions.h"  //This mode is similar to dataMonitorChart, so disable it by default.

//Sys Config
#include "src/sysConfig.h"
sysConfig sysConfig;
sysConfig_data tmp_cfg_data;

enum function_mode {
  dataMonitor,
  dataMonitorChart,
  dataMonitorCount,
  dataChart,
  // serialMonitor,
  // pwmOutput,
  // analogInputMonitor,
};

volatile function_mode current_function_mode = dataMonitorChart;
volatile bool functionModeChangeRequested = true;  //a flag to indicate a mode change is requested
volatile bool singleModeDisplayChannel_ChangeRequested = false;
uint8_t singleModeDisplayChannel = 0;


//Current sensor
#include "src/INA3221Sensor.h"
INA3221Sensor inaSensor;

DualChannelData latestSensorData;  // Add a global variable to store the latest sensor data
DualChannelData oldSensorData;

float avgS[2] = { 0 }, avgM[2] = { 0 }, avgA[2] = { 0 }, peak[2] = { 0 };
unsigned long dataCount[2] = { 0 };
unsigned long lastUpdate = 0;
double sumS[2] = { 0 }, sumM[2] = { 0 }, sumA[2] = { 0 };
unsigned long countS[2] = { 0 }, countM[2] = { 0 };

//Tasks
#if defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C6)
    #define STACK_SIZE_UI 4096
    #define STACK_SIZE_SERIAL 4096
    #define STACK_SIZE_DIAL 2048
    #define STACK_SIZE_SENSOR 2048
#else
    #define STACK_SIZE_UI 2048
    #define STACK_SIZE_SERIAL 1024
    #define STACK_SIZE_DIAL 1024
    #define STACK_SIZE_SENSOR 1024
#endif

StaticTask_t xTaskBuffer_UI;
StackType_t xStack_UI[STACK_SIZE_UI];

StaticTask_t xTaskBuffer_Serial;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];

StaticTask_t xTaskBuffer_Dial;
StackType_t xStack_Dial[STACK_SIZE_DIAL];

StaticTask_t xTaskBuffer_Sensor;
StackType_t xStack_Sensor[STACK_SIZE_SENSOR];

SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xMutexBuffer;

TaskHandle_t xUITaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
TaskHandle_t xDialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;

//Watchdog
#define WATCHDOG_TIMEOUT 5000  // 5 seconds


// Add the new sensor update task
void sensorUpdateTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(20);  // Update every 20ms

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    #if !defined(SEEED_XIAO_C3) && !defined(SEEED_XIAO_S3) && !defined(SEEED_XIAO_C6)
        Watchdog.reset();
    #endif

    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Read sensor data
      DualChannelData newSensorData = inaSensor.readCurrentSensors();

      // Update oldSensorData if values have changed
      if (newSensorData.channel0.isDirty || newSensorData.channel1.isDirty) {
        oldSensorData = latestSensorData;
      }

      // Update latestSensorData
      latestSensorData = newSensorData;

      xSemaphoreGive(xSemaphore);
    }
  }
}

void updateUITask(void *pvParameters) {
  (void)pvParameters;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(50); //50=50ms

  while (1) {
    #if !defined(SEEED_XIAO_C3) && !defined(SEEED_XIAO_S3) && !defined(SEEED_XIAO_C6)
        Watchdog.reset();
    #endif

    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    // Use the latest sensor data
    DualChannelData sensorData = latestSensorData;

    xSemaphoreGive(xSemaphore);

    if (functionModeChangeRequested) {
      handleFunctionModeChange(sensorData);
    }

    switch (current_function_mode) {
      case dataMonitor:
        handleDataMonitorMode(sensorData);
        break;
      case dataChart:
        handleDataChartMode(sensorData);
        break;
      case dataMonitorChart:
        handleDataMonitorChartMode(sensorData);
        break;
      case dataMonitorCount:
        handleDataMonitorCountMode(sensorData);
        break;
    }
  }
}

void handleFunctionModeChange(const DualChannelData &sensorData) {
  functionModeChangeRequested = false;
  tft.fillScreen(color_Background);
  switch (current_function_mode) {
    case dataMonitor:
      dataMonitor_initUI(tft_Rotation);
      dataMonitor_updateData(sensorData, 1);//force update
      break;
    case dataChart:
      tft_Rotation = 1;
      dataChart_changeRotation(tft_Rotation);
      dataChart_initUI(singleModeDisplayChannel);
      break;
    case dataMonitorChart:
      dataMonitorChart_initUI(singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data);
      dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data, 1);
      break;
    case dataMonitorCount:
      dataMonitorCount_initUI(singleModeDisplayChannel, tft_Rotation);
      dataMonitorCount_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, 1);
      break;
  }
}

void handleDataMonitorMode(const DualChannelData &sensorData) {
  if (rotationChangeRequested) {
    rotationChangeRequested = false;
    dataMonitor_update_chAB_xy_by_Rotation(tft_Rotation);
    dataMonitor_changeRotation(tft_Rotation, oldSensorData);
  }
  dataMonitor_updateData(sensorData);
}

void handleDataChartMode(const DualChannelData &sensorData) {
  if (singleModeDisplayChannel_ChangeRequested) {
    singleModeDisplayChannel_ChangeRequested = false;
    tft.fillScreen(color_Background);
    dataChart_initUI(singleModeDisplayChannel);
  }
  dataChart_updateData(sensorData, singleModeDisplayChannel);
}

void handleDataMonitorChartMode(const DualChannelData &sensorData) {
  if (singleModeDisplayChannel_ChangeRequested) {
    singleModeDisplayChannel_ChangeRequested = false;
    dataMonitorChart_initUI(singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data);
    dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data, 1);
  }
  if (rotationChangeRequested) {
    rotationChangeRequested = false;
    dataMonitorChart_changeRotation(sensorData, singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data);
  }
  dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, sysConfig.cfg_data);
}

void handleDataMonitorCountMode(const DualChannelData &sensorData) {
  if (singleModeDisplayChannel_ChangeRequested) {
    singleModeDisplayChannel_ChangeRequested = false;
    dataMonitorCount_initUI(singleModeDisplayChannel, tft_Rotation);
    dataMonitorCount_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, 1);
  }
  if (rotationChangeRequested) {
    rotationChangeRequested = false;
    dataMonitorCount_changeRotation(sensorData, singleModeDisplayChannel, tft_Rotation);
  }
  dataMonitorCount_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, 0);
}

void serialPrintTask(void *pvParameters) {
    (void)pvParameters;
    TickType_t xLastPrintTime = 0;
    const TickType_t xPrintIntervals[] = {
        pdMS_TO_TICKS(1000),  // 0 - 1000ms
        pdMS_TO_TICKS(500),   // 1 - 500ms
        pdMS_TO_TICKS(100),   // 2 - 100ms
        pdMS_TO_TICKS(50),    // 3 - 50ms
        pdMS_TO_TICKS(10)     // 4 - 10ms
    };

    uint8_t intervalIndex = min(sysConfig.cfg_data.serial_printInterval, (uint8_t)4);
    const TickType_t xPrintInterval = xPrintIntervals[intervalIndex];
    const bool serialEnabled = sysConfig.cfg_data.serial_enable;
    const uint8_t serialMode = sysConfig.cfg_data.serial_mode;

    while (1) {
        #if !defined(SEEED_XIAO_C3) && !defined(SEEED_XIAO_S3) && !defined(SEEED_XIAO_C6)
            Watchdog.reset();
        #endif

        vTaskDelay(pdMS_TO_TICKS(10));  // Give other tasks a chance to run

        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastPrintTime) >= xPrintInterval) {
            if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
                DualChannelData sensorData = latestSensorData;
                xSemaphoreGive(xSemaphore);
                
                if (Serial && serialEnabled) {
                    char buffer[150];  // Pre-allocate buffer for string formatting
                    if (serialMode == 0) {
                        snprintf(buffer, sizeof(buffer), 
                            "A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
                            sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000,
                            sensorData.channel0.busCurrent, sensorData.channel0.busPower,
                            sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000,
                            sensorData.channel1.busCurrent, sensorData.channel1.busPower);
                    } else {
                        snprintf(buffer, sizeof(buffer),
                            "V0:%.2f,I0:%.2f,P0:%.2f,V1:%.2f,I1:%.2f,P1:%.2f\n",
                            sensorData.channel0.busVoltage, sensorData.channel0.busCurrent,
                            sensorData.channel0.busPower, sensorData.channel1.busVoltage,
                            sensorData.channel1.busCurrent, sensorData.channel1.busPower);
                    }
                    Serial.print(buffer);
                }
                xLastPrintTime = xCurrentTime;
            }
        }
    }
}

void dialReadTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t xLastReadTime = 0;
  const TickType_t xReadInterval = pdMS_TO_TICKS(50);  // Read every 50ms
  TickType_t pressStartTime = 0;
  const TickType_t longPressThreshold = pdMS_TO_TICKS(500);  // 0.5 second
  bool longPressDetected = false;
  bool shortPressHandled = false;

  while (1) {
    #if !defined(SEEED_XIAO_C3) && !defined(SEEED_XIAO_S3) && !defined(SEEED_XIAO_C6)
        Watchdog.reset();
    #endif

    TickType_t xCurrentTime = xTaskGetTickCount();
    if ((xCurrentTime - xLastReadTime) >= xReadInterval) {
      dialStatus = dial.readDialStatus();

      if (dialStatus == 3) {  // Pressed
        if (pressStartTime == 0) {
          pressStartTime = xCurrentTime;
          shortPressHandled = false;
        } else if ((xCurrentTime - pressStartTime) >= longPressThreshold && !longPressDetected) {
          longPressDetected = true;
          // Serial.println("Dial long pressed");
          longPress_Handler(current_function_mode);
          vTaskDelay(pdMS_TO_TICKS(300));  // Debounce delay
        }
      } else if (dialStatus == 0) {  // Reset
        if (lastDialStatus == 1) {   // Was Up
          // Serial.println("Dial released from Up");
          rotationChange_Handler(current_function_mode, 1);
          vTaskDelay(pdMS_TO_TICKS(50));   // Debounce delay
        } else if (lastDialStatus == 2) {  // Was Down
          // Serial.println("Dial released from Down");
          rotationChange_Handler(current_function_mode, 2);
          vTaskDelay(pdMS_TO_TICKS(50));  // Debounce delay
        } else if (pressStartTime != 0 && !longPressDetected && !shortPressHandled) {
          // This was a short press that just ended
          // Serial.println("Dial short pressed");
          shortPress_Handler(current_function_mode);
          shortPressHandled = true;
          vTaskDelay(pdMS_TO_TICKS(300));  // Debounce delay
        }
        pressStartTime = 0;
        longPressDetected = false;
      }

      lastDialStatus = dialStatus;
      xLastReadTime = xCurrentTime;
    }
    vTaskDelay(pdMS_TO_TICKS(10));  // Reduced delay for more frequent checks
  }
}

void rotationChange_Handler(function_mode currentMode, int dialStatus) {
  if (currentMode == dataMonitor || currentMode == dataMonitorChart || currentMode == dataMonitorCount) {
    rotationChangeRequested = true;
    tft_Rotation = (dialStatus == 1) ? (tft_Rotation + 1) % 4 : (tft_Rotation - 1 + 4) % 4;
    // Serial.println("Rotation changed to: " + String(tft_Rotation));  // Debug print
  } else if (currentMode == dataChart) {
    // Handle dataChart rotation if needed
    // Serial.println("Rotation in dataChart mode");  // Debug print
  }
}


//Short press is basicly for switching mode
//Mode change order: dataMonitor -> dataMonitorChart -> dataMonitorCount -> dataChart(skip) -> dataMonitor
void shortPress_Handler(function_mode currentMode) {
  functionModeChangeRequested = true;
  switch (currentMode) {
    case dataMonitor:
      current_function_mode = dataMonitorChart;
      break;
    case dataMonitorChart:
      current_function_mode = dataMonitorCount;
      break;
    case dataMonitorCount:
      current_function_mode = dataMonitor;
      break;
    case dataChart:
      current_function_mode = dataMonitor;  //skip this mode for now
      break;
  }
  // Serial.println("Function mode changed to " + String(current_function_mode));
}

//Long press is basicly for changing channel
void longPress_Handler(function_mode currentMode) {
  if (currentMode == dataMonitor) {
    // Serial.println("Data Monitor long pressed");
  } else if (currentMode == dataChart) {
    // Serial.println("Data Chart long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    // Serial.println("Channel changed to " + String(singleModeDisplayChannel));
  } else if (currentMode == dataMonitorChart) {
    // Serial.println("Data Monitor Chart long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    // Serial.println("Channel changed to " + String(singleModeDisplayChannel));
  } else if (currentMode == dataMonitorCount) {
    // Serial.println("Data Monitor Count long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    // Serial.println("Channel changed to " + String(singleModeDisplayChannel));
    //Reset the AvgA, AvgM, AvgS, Peak data when switch channel
    for (int i = 0; i < 2; i++) {
      avgA[i] = 0;
      avgM[i] = 0;
      avgS[i] = 0;
      peak[i] = 0;
      dataCount[i] = 0;
      sumA[i] = 0;
      sumM[i] = 0;
      sumS[i] = 0;
      countS[i] = 0;
      countM[i] = 0;
    }
  }
}

void setup(void) {
  //Dial init
  dial.init();

  // LCD init
  #if defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_C6)
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  #endif

  tft.setSPISpeed(24000000);      //24MHz
  tft.initR(INITR_GREENTAB);      // Init ST7735S 0.96inch display (160*80), Also need to modify the _colstart = 24 and _rowstart = 0 in Adafruit_ST7735.cpp>initR(uint8_t)
  tft.setRotation(tft_Rotation);  //Rotate the LCD 180 degree (0-3)
  tft.fillScreen(color_Background);

  // init serial
  Serial.begin(115200);
  Serial.println("--------[XIAO PowerBread boot]--------");

  //UI init
  // systemUI_init();
  // systemUI_bootScreen(); //skip boot screen

  //sysConfig init
  sysConfig.begin();
  sysConfig.init(0); //Pass 1 to force_write default config to EEPROM if needed, don't do it very frequently, or it will kill the EEPROM(SPI Flash)
  Serial.println(sysConfig.debugPrintOnSerial());

  //check if dial is turn down, if so, enter the config mode
  if (dial.readDialStatus() == 2) {
    Serial.println("Entering config mode");
    systemUI_sysConfig_init();
    tmp_cfg_data = sysConfig.cfg_data;  //copy cfg_data to tmp_cfg_data

    int cursor = 0;
    bool isSelected = false;
    int lastDialStatus = 0;
    bool exitConfig = false;

    while (!exitConfig) {
      systemUI_sysConfig_update(cursor, isSelected, tmp_cfg_data);
      dialStatus = dial.readDialStatus();

      if (isSelected) {
        // Modify selected value
        if (dialStatus == 1) {
          // Increase value
          incrementConfigValue(cursor, tmp_cfg_data);
        } else if (dialStatus == 2) {
          // Decrease value
          decrementConfigValue(cursor, tmp_cfg_data);
        }
      } else {
        // Navigate menu
        if (dialStatus == 2) {
          cursor++;
          if (cursor > 10) {
            cursor = 0;
          }
        } else if (dialStatus == 1) {
          cursor--;
          if (cursor < 0) {
            cursor = 10;
          }
        } else if (dialStatus == 3) {
          // Long press to exit configuration
          unsigned long pressStartTime = millis();
          while (dial.readDialStatus() == 3) {
            if (millis() - pressStartTime > 1000) {  // Long press for 1 second
              exitConfig = true;
              break;
            }
          }
        }
      }

      // Handle button press and release for selection toggle
      if (dialStatus == 3 && lastDialStatus != 3) {
        // Button just pressed
        isSelected = !isSelected;
      }

      lastDialStatus = dialStatus;
      delay(100);  // A delay to prevent multiple clicks from being too fast
    }

    // Save configuration if changes were made
    if (memcmp(&sysConfig.cfg_data, &tmp_cfg_data, sizeof(sysConfig_data)) != 0) {
      sysConfig.cfg_data = tmp_cfg_data;
      sysConfig.saveConfig(sysConfig.cfg_data);
      Serial.println("Configuration saved");
    }

    //print again to confirm the config is updated
    Serial.println(sysConfig.debugPrintOnSerial());


    //show the config saved message
    systemUI_MSG_savedConfig();
    delay(2000);
  }

  //Apply the cfg_data to the variables
  current_function_mode = (function_mode)sysConfig.cfg_data.default_mode;
  singleModeDisplayChannel = sysConfig.cfg_data.default_channel;
  float shuntResistorCHA = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;
  float shuntResistorCHB = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;
  //Serial-enable is applied in the serialPrintTask
  //Serial-baudRate is applied in the Serial.begin(), but now is only 115200
  //Serial-mode is applied in the serialPrintTask
  //Serial-printInterval is applied in the serialPrintTask
  //chart_updateInterval is applied in the dataMonitorChart_updateChartInterval()


  //Current sensor init
  pinMode(D4, INPUT_PULLUP); //SDA, set internal pull-up
  pinMode(D5, INPUT_PULLUP); //SCL, set internal pull-up


  if (!inaSensor.begin(shuntResistorCHA, shuntResistorCHB)) {
    while (1){
      // Print error message
      Serial.println("INA3221 initialization failed. Please check the wiring and try again.");
      delay(1000);

      // Since not all XIAO boards have built-in LED, so we don't use LED blink here.
      // Need to find another way to indicate the error besides Serial print. Maybe LCD screen?
      // Todo: Add LCD error message
    }
  }


  //semaphore init
  xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
  if (xSemaphore == NULL) {
    Serial.println("Error creating semaphore");
    while (1)
      ;
  }

  // Watchdog only for RP2040 & RP2350
  #if !defined(SEEED_XIAO_C3) && !defined(SEEED_XIAO_S3) && !defined(SEEED_XIAO_C6)
      int countdownMS = Watchdog.enable(WATCHDOG_TIMEOUT);
      Serial.print("Watchdog enabled with ");
      Serial.print(countdownMS);
      Serial.println("ms timeout");
  #endif

  // Create tasks (no watchdog references)
  xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", 
      STACK_SIZE_SENSOR, NULL, 4, xStack_Sensor, &xTaskBuffer_Sensor);

  xUITaskHandle = xTaskCreateStatic(updateUITask, "UI_Update", 
      STACK_SIZE_UI, NULL, 3, xStack_UI, &xTaskBuffer_UI);

  xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", 
      STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);

  xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", 
      STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);

  Serial.println("-----------[Boot info end]------------");

  //it is a way to start scheduler, but this will cause crash, so we don't use it, and marked as comment
  //vTaskStartScheduler();
}

void loop() {
  // Empty, as tasks will handle the main functionality
}

// Add this function at the end of your file
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  //it use for checking task stack overflow when debugging
  Serial.print("Stack Overflow in task: ");
  Serial.println(pcTaskName);
  while (1)
    ;
}


extern "C" void vApplicationIdleHook(void) {
  //it use for checking task states when debugging
}

void vApplicationTickHook(void) {
  //it use for checking task states when debugging
}
