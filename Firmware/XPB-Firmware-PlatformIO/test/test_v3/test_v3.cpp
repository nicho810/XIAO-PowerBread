#include <Arduino.h>
#include "boardConfig.h"
#include "global_variables.h"

// RTOS Tasks
#include "task_serial.h"       // serial task
#include "task_sensor.h" // sensor update task
#include "task_lvgl.h"   // lvgl task


// Current sensor
std::vector<currentSensorData> cSensorData;
#if defined(Proj_XIAOPowerBread)
#include "CurrentSensor_INA3221.h"
CurrentSensor_INA3221 c_Sensor;
#elif defined(Proj_XIAOPowerMonitor)
#include "CurrentSensor_INA226.h"
cSensor_2ch_ina226 currentSensor_2ch(currentSensor_0_address, currentSensor_1_address, {currentSensor_0_shuntR_0, currentSensor_1_shuntR_0}, 4);
#endif

// LCD & LVGL
#include <LovyanGFX.h>
#if defined(Proj_XIAOPowerBread)
#include <LGFX_096_XPB.hpp>
#elif defined(Proj_XIAOPowerMonitor)
#include <LGFX_114_XPM.hpp>
#endif
LGFX tft;// Create LGFX instance
#include "lvgl_func.h"


// FreeRTOS Task Declarations
#if defined(SEEED_XIAO_ESP32C3)
// #define STACK_SIZE_UI 8192        // Increased from 4096
#define STACK_SIZE_SERIAL 2048    // Reduced from 4096
// #define STACK_SIZE_DIAL 2048      // Reduced from 4096
#define STACK_SIZE_SENSOR 2048    // Reduced from 4096
#define STACK_SIZE_LVGL 2048      // Reduced from 4096
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#define STACK_SIZE_UI 2048
#define STACK_SIZE_SERIAL 1024
#define STACK_SIZE_DIAL 1024
#define STACK_SIZE_SENSOR 1024
#endif

// Task buffers and stacks
StaticTask_t xTaskBuffer_Serial, xTaskBuffer_Sensor, xTaskBuffer_Lvgl;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];
StackType_t xStack_Sensor[STACK_SIZE_SENSOR];
StackType_t xStack_Lvgl[STACK_SIZE_LVGL];

// Task handles
TaskHandle_t xLvglTaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;

// Other semaphores
SemaphoreHandle_t lvglMutex = NULL;
SemaphoreHandle_t xSemaphore = NULL;


// Modify your task creation priorities
#define TASK_PRIORITY_LVGL       3  // Keep same as UI init
#define TASK_PRIORITY_SENSOR     2  // Keep as is
#define TASK_PRIORITY_SERIAL     1  // Keep as is

void setup(void)
{
    // Serial Init
    Serial.begin(115200);
    delay(1000);
    Serial.println("==========[Boot Info]===========");

    // Current Sensor Init
    Wire.begin();
    currentSensor_2ch.initialize();

    //init LCD
    lvgl_init();

    // Create tasks with proper return type checking
    TaskHandle_t taskHandle;

    // Create the semaphore
    xSemaphore = xSemaphoreCreateMutex();
    if (xSemaphore == NULL) {
        Serial.println("ERROR: Failed to create semaphore!");
        while(1) delay(100);
    }
    Serial.println("> Semaphore created successfully");

    // Create the lvglMutex
    lvglMutex = xSemaphoreCreateMutex();
    if (lvglMutex == NULL) {
        Serial.println("ERROR: Failed to create lvgl mutex!");
        while(1) delay(100);
    }
    Serial.println("> Lvgl mutex created successfully");
    

    // Create sensor task first as it's highest priority
    taskHandle = xTaskCreateStatic(sensorTask, "Sensor_Update", 
        STACK_SIZE_SENSOR, NULL, TASK_PRIORITY_SENSOR, 
        xStack_Sensor, &xTaskBuffer_Sensor);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create sensor task!");
        while(1) delay(100);
    }
    Serial.println("> Sensor task created successfully");

    // Create lvgl task
    taskHandle = xTaskCreateStatic(lvglTask, "Lvgl_Task", 
        STACK_SIZE_LVGL, NULL, TASK_PRIORITY_LVGL, 
        xStack_Lvgl, &xTaskBuffer_Lvgl);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create lvgl task!");
        while(1) delay(100);
    }
    Serial.println("> Lvgl task created successfully");

    // Create serial task
    taskHandle = xTaskCreateStatic(serialTask, "Serial_Print", 
        STACK_SIZE_SERIAL, NULL, TASK_PRIORITY_SERIAL, 
        xStack_Serial, &xTaskBuffer_Serial);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create serial task!");
        while(1) delay(100);
    }
    Serial.println("> Serial task created successfully");


    // Start the scheduler
    // vTaskStartScheduler(); //Note: no need to call this, it will cause a crash, keep this note here as reminder.
}

void loop()
{
    // Empty - tasks handle everything
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // it use for checking task stack overflow when debugging
    Serial.print("Stack Overflow in task: ");
    Serial.println(pcTaskName);
    while (1)
        ;
}

#if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)

extern "C" void vApplicationIdleHook(void)
{
    // it use for checking task states when debugging
}

void vApplicationTickHook(void)
{
    // it use for checking task states when debugging
}

#elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3) || defined(SEEED_XIAO_ESP32C6)
//no need for these hooks on ESP32 series
#endif