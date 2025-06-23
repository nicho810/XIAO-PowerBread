#include <Arduino.h>
#include "boardConfig.h"


// RTOS Tasks
#include "task_serial.h"       // serial task
#include "task_sensor.h" // sensor update task
#include "task_lvgl.h"   // lvgl task
#include "task_input.h"  // input task
#include "task_ui.h"    // ui task


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
lv_obj_t *ui_container = NULL;   // Global container for the chart UI

// Input Device
#if defined(Proj_XIAOPowerBread)
#include "input_Dial.h" //todo: add dial input device
#elif defined(Proj_XIAOPowerMonitor)
#include "input_ButtonX3.h"
InputButtonX3 input_buttonX3(pin_button);
ButtonState_X3 buttonState_X3;
lv_indev_drv_t indev_drv; // Input device driver
#endif

//UI
#include "lvgl_ui.h"
UI_manager ui_manager;


// FreeRTOS Task Declarations
#if defined(SEEED_XIAO_ESP32C3)
#define STACK_SIZE_UI 8192      
#define STACK_SIZE_SERIAL 2048    
#define STACK_SIZE_SENSOR 2048   
#define STACK_SIZE_LVGL 2048    
#define STACK_SIZE_INPUT 2048   

#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#define STACK_SIZE_UI 2048
#define STACK_SIZE_SERIAL 1024
#define STACK_SIZE_SENSOR 1024
#endif

// Task buffers and stacks
StaticTask_t xTaskBuffer_Serial, xTaskBuffer_Sensor, xTaskBuffer_Lvgl, xTaskBuffer_Input, xTaskBuffer_UI;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];
StackType_t xStack_Sensor[STACK_SIZE_SENSOR];
StackType_t xStack_Lvgl[STACK_SIZE_LVGL]; 
StackType_t xStack_Input[STACK_SIZE_INPUT];
StackType_t xStack_UI[STACK_SIZE_UI];

// Task handles
TaskHandle_t xLvglTaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;
TaskHandle_t xInputTaskHandle = NULL;
TaskHandle_t xUITaskHandle = NULL;

// Queues (replacing semaphores)
QueueHandle_t sensorDataQueue_serial = NULL;
QueueHandle_t sensorDataQueue_ui = NULL;
QueueHandle_t buttonEventQueue = NULL;
SemaphoreHandle_t lvglMutex = NULL; // Keep this for LVGL operations

// Modify your task creation priorities
#define TASK_PRIORITY_LVGL       3  
#define TASK_PRIORITY_UI         4 
#define TASK_PRIORITY_SENSOR     2
#define TASK_PRIORITY_SERIAL     1  
#define TASK_PRIORITY_INPUT      1


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

    // Create the sensor data queue for serial
    sensorDataQueue_serial = xQueueCreate(2, sizeof(SensorDataMessage));
    if (sensorDataQueue_serial == NULL) {
        Serial.println("ERROR: Failed to create sensor data queue_serial!");
        while(1) delay(100);
    }
    Serial.println("> Sensor data queue_serial created successfully");

    // Create the sensor data queue for UI
    sensorDataQueue_ui = xQueueCreate(2, sizeof(SensorDataMessage));
    if (sensorDataQueue_ui == NULL) {
        Serial.println("ERROR: Failed to create sensor data queue_ui!");
        while(1) delay(100);
    }
    Serial.println("> Sensor data queue_ui created successfully");

    // Create the button event queue
    buttonEventQueue = xQueueCreate(1, sizeof(ButtonEventMessage));
    if (buttonEventQueue == NULL) {
        Serial.println("ERROR: Failed to create button event queue!");
        while(1) delay(100);
    }
    Serial.println("> Button event queue created successfully");

    // Create the lvglMutex (keep this for LVGL operations)
    lvglMutex = xSemaphoreCreateMutex();
    if (lvglMutex == NULL) {
        Serial.println("ERROR: Failed to create lvgl mutex!");
        while(1) delay(100);
    }
    Serial.println("> Lvgl mutex created successfully");
    
    // Create input task
    taskHandle = xTaskCreateStatic(inputTask, "Input_Task", 
        STACK_SIZE_INPUT, NULL, TASK_PRIORITY_INPUT, 
        xStack_Input, &xTaskBuffer_Input);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create input task!");
        while(1) delay(100);
    }
    Serial.println("> Input task created successfully");

    // Create sensor task first as it's highest priority
    taskHandle = xTaskCreateStatic(sensorTask, "Sensor_Update", 
        STACK_SIZE_SENSOR, NULL, TASK_PRIORITY_SENSOR, 
        xStack_Sensor, &xTaskBuffer_Sensor);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create sensor task!");
        while(1) delay(100);
    }
    Serial.println("> Sensor task created successfully");

    //Dont need this task for now, lvgl task is handled by ui task
    /*
    // Create lvgl task
    taskHandle = xTaskCreateStatic(lvglTask, "Lvgl_Task", 
        STACK_SIZE_LVGL, NULL, TASK_PRIORITY_LVGL, 
        xStack_Lvgl, &xTaskBuffer_Lvgl);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create lvgl task!");
        while(1) delay(100);
    }
    Serial.println("> Lvgl task created successfully");
    */

    // Create UI task
    taskHandle = xTaskCreateStatic(uiTask, "UI_Task", 
        STACK_SIZE_UI, NULL, TASK_PRIORITY_UI, 
        xStack_UI, &xTaskBuffer_UI);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create UI task!");
        while(1) delay(100);
    }
    Serial.println("> UI task created successfully");

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
