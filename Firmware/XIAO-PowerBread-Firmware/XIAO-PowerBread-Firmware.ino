#include <cstdint>
//LCD init
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h>

//RTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Add watchdog include
#include <Adafruit_SleepyDog.h>

//LCD
#define TFT_CS -1  //CS is always connected to ground in this project.
#define TFT_RST D3
#define TFT_DC D9
#define TFT_MOSI D10
#define TFT_SCLK D8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// LCD Rotation
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 2;  // default rotation.

//UIs
#include "systemUI_functions.h"
#include "dataMonitor_functions.h"
#include "dataMonitorChart_functions.h"
#include "dataMonitorCount_functions.h"
#include "dataChart_functions.h" //This mode is similar to dataMonitorChart, so disable it by default. Enable it if need a big chart.



enum function_mode {
  dataMonitor,
  dataMonitorChart,
  dataMonitorCount,
  dataChart,
  // serialMonitor,
  // pwmOutput,
  // analogInputMonitor,
};

volatile function_mode current_function_mode = dataMonitor;
volatile bool functionModeChangeRequested = true;  //a flag to indicate a mode change is requested
volatile bool singleModeDisplayChannel_ChangeRequested = false;
uint8_t singleModeDisplayChannel = 0;


//Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;

float avgS[2] = {0}, avgM[2] = {0}, avgA[2] = {0}, peak[2] = {0};
unsigned long dataCount[2] = {0};
unsigned long lastUpdate = 0;
double sumS[2] = {0}, sumM[2] = {0}, sumA[2] = {0};
unsigned long countS[2] = {0}, countM[2] = {0};

//Dial wheel switch
volatile int dialStatus = 0;  // 0: reset, 1: up, 2: down, 3: press, 4: long press
#define dial_adc A2
bool dial_enable = true;
volatile uint16_t dialValue = 0;  // Variable to store ADC value

void dial_init() {
  pinMode(dial_adc, INPUT);
  analogReadResolution(12);  // Set the ADC resolution to 10 bits
}

int read_dial() {
  return analogRead(dial_adc);
}


#define STACK_SIZE_UI 2048
StaticTask_t xTaskBuffer_UI;
StackType_t xStack_UI[STACK_SIZE_UI];

#define STACK_SIZE_SERIAL 1024
StaticTask_t xTaskBuffer_Serial;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];

#define STACK_SIZE_DIAL 1024
StaticTask_t xTaskBuffer_Dial;
StackType_t xStack_Dial[STACK_SIZE_DIAL];

#define STACK_SIZE_SENSOR 1024
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

// Add a global variable to store the latest sensor data
DualChannelData latestSensorData;
DualChannelData oldSensorData; //todo, use this to replace old_chA_v, old_chA_a, old_chA_w, old_chB_v, old_chB_a, old_chB_w

// Add the new sensor update task
void sensorUpdateTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(20);  // Update every 20ms

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    // Reset the watchdog timer
    Watchdog.reset();

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
  const TickType_t xFrequency = pdMS_TO_TICKS(50);

  while (1) {
    // Reset the watchdog timer
    Watchdog.reset();

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
      dataMonitor_ChannelInfoUpdate(0, oldSensorData.channel0.busVoltage, oldSensorData.channel0.busCurrent, oldSensorData.channel0.busPower, -1, -1, -1, color_Text);
      dataMonitor_ChannelInfoUpdate(1, oldSensorData.channel1.busVoltage, oldSensorData.channel1.busCurrent, oldSensorData.channel1.busPower, -1, -1, -1, color_Text);
      break;
    case dataChart:
      tft_Rotation = 1;
      dataChart_changeRotation(tft_Rotation);
      dataChart_initUI(singleModeDisplayChannel);
      break;
    case dataMonitorChart:
      dataMonitorChart_initUI(singleModeDisplayChannel, tft_Rotation);
      dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, 1);
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
    dataMonitorChart_initUI(singleModeDisplayChannel, tft_Rotation);
    dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation, 1);
  }
  if (rotationChangeRequested) {
    rotationChangeRequested = false;
    dataMonitorChart_changeRotation(sensorData, singleModeDisplayChannel, tft_Rotation);
  }
  dataMonitorChart_updateData(sensorData, singleModeDisplayChannel, tft_Rotation);
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
  const TickType_t xPrintInterval = pdMS_TO_TICKS(1000);
  while (1) {
    // Reset the watchdog timer
    Watchdog.reset();

    TickType_t xCurrentTime = xTaskGetTickCount();
    if ((xCurrentTime - xLastPrintTime) >= xPrintInterval) {
      if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Use the latest sensor data
        DualChannelData sensorData = latestSensorData;
        xSemaphoreGive(xSemaphore);
        // Only print if Serial is available
        if (Serial) {
          Serial.printf("A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
                        sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000, sensorData.channel0.busCurrent, sensorData.channel0.busPower,
                        sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000, sensorData.channel1.busCurrent, sensorData.channel1.busPower);
        }

        xLastPrintTime = xCurrentTime;
      }
      // If we couldn't acquire the semaphore, we'll try again next time
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // Increase delay to 100ms
  }
}

void dialReadTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t xLastReadTime = 0;
  const TickType_t xReadInterval = pdMS_TO_TICKS(50);  // Read every 50ms
  uint16_t lastDialValue = 0;
  int lastDialStatus = 0;
  TickType_t pressStartTime = 0;
  const TickType_t longPressThreshold = pdMS_TO_TICKS(500);  // 0.5 second
  bool longPressDetected = false;
  bool shortPressHandled = false;

  while (1) {
    // Reset the watchdog timer
    Watchdog.reset();

    TickType_t xCurrentTime = xTaskGetTickCount();
    if ((xCurrentTime - xLastReadTime) >= xReadInterval) {
      dialValue = read_dial();
      if (dialValue != lastDialValue) {
        if (dialValue >= 300 && dialValue < 800) {
          if (pressStartTime == 0) {
            pressStartTime = xCurrentTime;
            dialStatus = 3;  // Set to press, but don't handle it yet
            shortPressHandled = false;
          } else if ((xCurrentTime - pressStartTime) >= longPressThreshold && !longPressDetected) {
            dialStatus = 4;  // Long press
            longPressDetected = true;
            // Serial.println("Dial long pressed");
            longPress_Handler(current_function_mode);
            vTaskDelay(pdMS_TO_TICKS(300));  // Debounce delay
          }
        } else if (dialValue >= 800 && dialValue < 1500) {
          dialStatus = 2;
          pressStartTime = 0;
          longPressDetected = false;
          shortPressHandled = false;
        } else if (dialValue >= 1500 && dialValue < 2000) {
          dialStatus = 1;
          pressStartTime = 0;
          longPressDetected = false;
          shortPressHandled = false;
        } else {
          if (pressStartTime != 0 && !longPressDetected && !shortPressHandled) {
            // This was a short press that just ended
            // Serial.println("Dial short pressed");
            shortPress_Handler(current_function_mode);// Handle short press action here
            shortPressHandled = true;
            vTaskDelay(pdMS_TO_TICKS(300));  // Debounce delay
          }
          dialStatus = 0;  // Reset status for other values
          pressStartTime = 0;
          longPressDetected = false;
        }

        // Handle rotation changes
        if (dialStatus == 0 && (lastDialStatus == 1 || lastDialStatus == 2)) {
          rotationChange_Handler(current_function_mode, lastDialStatus); //handle rotation change based on lastDialStatus and currentMode
        }

        //reset lastDialStatus and lastDialValue
        lastDialStatus = dialStatus;
        lastDialValue = dialValue;
      }
      xLastReadTime = xCurrentTime;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void rotationChange_Handler(function_mode currentMode, int lastDialStatus) {
  if (currentMode == dataMonitor) {
    rotationChangeRequested = true;
    tft_Rotation = (lastDialStatus == 1) ? (tft_Rotation + 1) % 4 : (tft_Rotation - 1 + 4) % 4; 
  } else if (currentMode == dataChart) {
    //do nothing here for now, but will change rotation later
  } else if (currentMode == dataMonitorChart) {
    rotationChangeRequested = true;
    tft_Rotation = (lastDialStatus == 1) ? (tft_Rotation + 1) % 4 : (tft_Rotation - 1 + 4) % 4; 
  } else if (currentMode == dataMonitorCount) {
    rotationChangeRequested = true;
    tft_Rotation = (lastDialStatus == 1) ? (tft_Rotation + 1) % 4 : (tft_Rotation - 1 + 4) % 4; 
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
      current_function_mode = dataMonitor; //skip this mode for now
      break;
  }
  
  Serial.println("Function mode changed to " + String(current_function_mode));
}

//Long press is basicly for changing channel
void longPress_Handler(function_mode currentMode) {
  if (currentMode == dataMonitor) {
    Serial.println("Data Monitor long pressed");
  } else if (currentMode == dataChart) {
    // Serial.println("Data Chart long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    Serial.println("Channel changed to " + String(singleModeDisplayChannel));
  } else if (currentMode == dataMonitorChart) {
    // Serial.println("Data Monitor Chart long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    Serial.println("Channel changed to " + String(singleModeDisplayChannel));
  } else if (currentMode == dataMonitorCount) {
    // Serial.println("Data Monitor Count long pressed");
    //Switch channel
    singleModeDisplayChannel = (singleModeDisplayChannel == 0) ? 1 : 0;
    singleModeDisplayChannel_ChangeRequested = true;
    Serial.println("Channel changed to " + String(singleModeDisplayChannel));
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
  // init serial
  Serial.begin(115200);
  Serial.println(F("Hello! XIAO PowerBread."));

  // LCD init
  tft.setSPISpeed(24000000);      //24MHz
  tft.initR(INITR_GREENTAB);      // Init ST7735S 0.96inch display (160*80), Also need to modify the _colstart = 24 and _rowstart = 0 in Adafruit_ST7735.cpp>initR(uint8_t)
  tft.setRotation(tft_Rotation);  //Rotate the LCD 180 degree (0-3)
  tft.fillScreen(color_Background);

  //UI init
  systemUI_init();
  systemUI_bootScreen();


  //Current sensor init
  if (!inaSensor.begin()) {
    Serial.println("INA3221 initialization failed. Fix and Reboot");
    while (1)
      ;  // Halt execution
  }

  //Dial init
  dial_init();

  //semaphore init
  xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
  if (xSemaphore == NULL) {
    Serial.println("Error creating semaphore");
    while (1)
      ;
  }

  // Initialize and start the watchdog
  int countdownMS = Watchdog.enable(WATCHDOG_TIMEOUT);
  Serial.print("Watchdog enabled with ");
  Serial.print(countdownMS);
  Serial.println("ms timeout");


  //Create sensor tasks
  Serial.println("Creating Sensor Task");
  xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", STACK_SIZE_SENSOR, NULL, 4, xStack_Sensor, &xTaskBuffer_Sensor);
  if (xSensorTaskHandle == NULL) {
    Serial.println("Sensor Task creation failed");
    while (1);
  }
  Serial.println("Sensor Task created successfully");

  //Create UI tasks
  Serial.println("Creating UI Task");
  xUITaskHandle = xTaskCreateStatic(updateUITask, "UI_Update", STACK_SIZE_UI, NULL, 3, xStack_UI, &xTaskBuffer_UI);
  if (xUITaskHandle == NULL) {
    Serial.println("UI Task creation failed");
    while (1)
      ;
  }
  Serial.println("UI Task created successfully");

  //Create dial tasks
  Serial.println("Creating Dial Task");
  xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);
  if (xDialTaskHandle == NULL) {
    Serial.println("Dial Task creation failed");
    while (1)
      ;
  }
  Serial.println("Dial Task created successfully");

  // Create serial tasks
  Serial.println("Creating Serial Task");
  xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);
  if (xSerialTaskHandle == NULL) {
    Serial.println("Serial Task creation failed");
    while (1)
      ;
  }
  Serial.println("Serial Task created successfully");


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

