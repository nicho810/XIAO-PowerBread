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
 * - Arduino-Pico Core: https://github.com/earlephilhower/arduino-pico
 * - Arduino-ESP32: https://github.com/espressif/arduino-esp32
 * - INA3221_RT Library: https://github.com/RobTillaart/INA3221_RT/tree/master
 * - LVGL: https://github.com/lvgl/lvgl
 * - LovyanGFX: https://github.com/lovyan03/LovyanGFX
 *
 * About the LCD Driver Library:
 * A modified version of Adafruit_ST7735&Adafruit_GFX library for v1.x
 * LovyanGFX is used starting from v2.x to make UI response faster.
 * We are grateful to the developers and contributors of these libraries.
 *
 * Licensed under the MIT License
 */

//=================================================================
// IMPORTANT: Make sure to define the board type in boardConfig.h !!!
//=================================================================

// Board
#include "boardConfig.h"
#include <Arduino.h>

// RTOS Tasks
#include "task_dialRead.h"     // dial read task
#include "task_serial.h"       // serial task
#include "task_sensorUpdate.h" // sensor update task
#include "task_lvgl.h"         // LVGL task
// LCD
#include <LovyanGFX.h>
// #include <LGFX_096_XPB.hpp>
#include <LGFX_114_XPM.hpp>
#include <lvgl.h>
#include "lvgl_func.h"    // LVGL flush and keyboard read function
LGFX tft;                 // Create LGFX instance
static LGFX_Sprite sprite(&tft); // Create LGFX_Sprite instance

// LVGL UI & Input Device Declaration
lv_obj_t *ui_container = NULL;   // Global container for the chart UI
static lv_indev_drv_t indev_drv; // Input device driver


// UIs
#include "lvgl_ui.h"
#include "lvgl_ui_updateFunc.h"


// Dial Switch
#include "dialSwitch.h"
DialFunction dial;
volatile int dialStatus = 0; // 0: reset, 1: up, 2: down, 3: press, 4: long press
volatile int lastDialStatus = 0;

// System Config
#include "sysConfig.h"
SysConfig sysConfig;
sysConfig_data tmp_cfg_data;
// sysConfig_data cfg_data_default; // default config data

// Config Mode functions
#include "configMode.h"

// Global variables
ConfigMode configMode;
SemaphoreHandle_t configStateMutex = NULL;
float shuntResistorCHA = 0.0f;
float shuntResistorCHB = 0.0f;
volatile function_mode current_functionMode = Mode_1;  //mode_1: basic monitor, mode_2: chart, mode_3: count
volatile bool functionMode_ChangeRequested = true;
volatile bool highLightChannel_ChangeRequested = false;
uint8_t highLightChannel = 0;
uint8_t forceUpdate_flag = 0;

// LCD Rotation variables
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 0; // default rotation.

// Current sensor
#if defined(Proj_XIAOPowerBread)
#include "CurrentSensor_INA3221.h"
CurrentSensor_INA3221 c_Sensor;
#elif defined(Proj_XIAOPowerMonitor)
#include "CurrentSensor_INA226.h"
CurrentSensor_DualINA226 c_Sensor;
#endif

DualChannelData latestSensorData;                                 // Add a global variable to store the latest sensor data
float avgS[2] = {0}, avgM[2] = {0}, avgH[2] = {0}, peak[2] = {0}; // Average values for each channel

// FreeRTOS Task Declarations
#if defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3) || defined(SEEED_XIAO_ESP32C6)
#define STACK_SIZE_UI 4096
#define STACK_SIZE_SERIAL 4096
#define STACK_SIZE_DIAL 4096
#define STACK_SIZE_SENSOR 4096
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#define STACK_SIZE_UI 2048
#define STACK_SIZE_SERIAL 1024
#define STACK_SIZE_DIAL 1024
#define STACK_SIZE_SENSOR 1024
#endif

// Task buffers and stacks
StaticTask_t xTaskBuffer_UI, xTaskBuffer_Serial, xTaskBuffer_Dial, xTaskBuffer_Sensor;
StackType_t xStack_UI[STACK_SIZE_UI];
StackType_t xStack_Serial[STACK_SIZE_SERIAL];
StackType_t xStack_Dial[STACK_SIZE_DIAL];
StackType_t xStack_Sensor[STACK_SIZE_SENSOR];

// Task handles
TaskHandle_t xLvglTaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
TaskHandle_t xDialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;

// Other semaphores
SemaphoreHandle_t lvglMutex = NULL;
SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xMutexBuffer;

//Variable for 
volatile bool configModeExitRequested = false;

// Add a timeout mechanism to the main loop waiting for config mode exit
unsigned long configModeStartTime = millis();
const unsigned long CONFIG_MODE_TIMEOUT = 60000; // 1 minute timeout

void setup(void)
{
    // Serial Init
    Serial.begin(115200);
    delay(3000);
    Serial.println("==========[XIAO-PowerBread Boot Info]===========");

    // Dial init
    dial.init();

    // Current Sensor Init
    if (!c_Sensor.begin()){
        while (1){
            Serial.println("Current Sensor initialization failed. Please check the wiring and try again."); // Print error message
            delay(1000);
            // Todo: Add LCD error message
        }
    }

    // LCD Init
    if (!tft.begin()) {  // Add error checking for LCD initialization
        Serial.println("LCD initialization failed!");
        while(1) delay(100);
    }
    tft.setColorDepth(16);         // RGB565
    tft.setRotation(tft_Rotation); // Set initial hardware rotation = 0
    tft.fillScreen(0x0000);        // Black screen

    // LVGL LCD Init
    lv_init();
    static lv_disp_draw_buf_t draw_buf;
    const int buf1_size = screen_width * (screen_width/2); // half screen buffer size
    static lv_color_t buf1[buf1_size]; // buffer 1
    static lv_color_t buf2[buf1_size]; // buffer 2
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf1_size); // Initialize the display buffer with dual buffering, decrease the buffer if want to save memory.
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screen_width;
    disp_drv.ver_res = screen_height;
    disp_drv.flush_cb = lvgl_disp_flush; // attach the flush callback function
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0; // 0=Enable partial updates for better performance
    disp_drv.direct_mode = 0;  // Direct mode is not supported, set to 0
    disp_drv.antialiasing = 1; // Enable antialiasing for better quality but slower performance (just a bit?)
    lv_disp_drv_register(&disp_drv);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);
    lv_disp_t *disp = lv_disp_get_default();
    disp->driver->monitor_cb = NULL; // LVGL optimization settings -> Disable monitor callback

    // LVGL Input Device Declaration
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv);

    // Set it as default input device (optional)
    if (kb_indev){
        lv_group_t *g = lv_group_create();
        lv_group_set_default(g);
        lv_indev_set_group(kb_indev, g);
    }

    // Create LVGL mutex
    lvglMutex = xSemaphoreCreateMutex();

    // Create config mode state mutex
    configStateMutex = xSemaphoreCreateMutex();

    // semaphore init
    xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    if (xSemaphore == NULL){
        Serial.println("Error creating semaphore");
        while (1) delay(100);
    }


    // Load the config data
    // Option 1: From default config data
    // sysConfig.loadConfig_from(cfg_data_default); 
    // Option 2: from EEPROM
    sysConfig.init_EEPROM(0); // 0=no force write, 1=force write
    sysConfig.loadConfig_from_EEPROM();

    // Print all config data
    Serial.println(sysConfig.output_all_config_data_in_String()); 

    // Create tasks first
    xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", STACK_SIZE_SENSOR, NULL, 3, xStack_Sensor, &xTaskBuffer_Sensor);
    xLvglTaskHandle = xTaskCreateStatic(lvglTask, "UI_Update", STACK_SIZE_UI, NULL, 4, xStack_UI, &xTaskBuffer_UI);
    xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);
    xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);


    // Check the key status for config mode entry
    if (dial.readDialStatus() == 2) { // When "Down" key is pressed, enter config mode
        configMode.enterConfigMode();
        tmp_cfg_data = sysConfig.cfg_data; // Copy the config data to tmp_cfg_data
        Serial.println("> Entering config mode.");

        // Initialize config mode UI and handle user inputs
        configMode.handleConfigMode(ui_container, sysConfig, tmp_cfg_data);

        // Block here until config mode is exited by the user
        while (configMode.configState.isActive) {
            // Check if exit was requested by another task
            if (configModeExitRequested) {
                Serial.println("> Processing config mode exit request");
                Serial.flush();
                
                // Clear the flag first
                configModeExitRequested = false;
                
                // Safe exit from the main thread
                configMode.exitConfigMode();
                
                Serial.println("> Config mode exited successfully");
                Serial.flush();
                
                // Break out of the waiting loop
                break;
            }
            
            // Timeout check
            if (millis() - configModeStartTime > CONFIG_MODE_TIMEOUT) {
                Serial.println("> Config mode timeout - forcing exit");
                Serial.flush();
                configMode.exitConfigMode();
                break;
            }
            
            // Just delay to allow other tasks to run
            delay(50);
        }

        // Save the config data to EEPROM
        sysConfig.saveConfig_to_EEPROM(tmp_cfg_data);
        Serial.println("> Config data saved to EEPROM.");
        sysConfig.loadConfig_from_EEPROM();
        Serial.println(sysConfig.output_all_config_data_in_String());
        Serial.flush();
        
        // Set flag to force UI refresh with the correct mode
        functionMode_ChangeRequested = true;
    }

    configMode.applyConfigData(sysConfig, shuntResistorCHA, shuntResistorCHB, highLightChannel, current_functionMode);
    c_Sensor.setParameter(10, 10);


    // // Apply config data
    // if (configMode.configState.isActive) {
    //     configMode.handleConfigMode(ui_container, sysConfig, tmp_cfg_data);

    //     configMode.exitConfigMode();
    //     Serial.println("> Exiting config mode.");
    //     Serial.flush();
    // } else {
    //     configMode.applyConfigData(sysConfig, shuntResistorCHA, shuntResistorCHB, highLightChannel, current_functionMode);
    //     inaSensor.setParameter(shuntResistorCHA, shuntResistorCHB);
    // }


    // Initialize default UI
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
        // Clean the screen first
        lv_obj_clean(lv_scr_act());
        
        Serial.println("Initializing UI...");
        Serial.flush();
        // Use the correct UI based on current function mode
        initUI(current_functionMode, ui_container, highLightChannel, latestSensorData, 0.0f, 0);

        Serial.println("UI initialized.");
        Serial.flush();
        // switch (current_functionMode) {
        // case dataMonitor:
        //     ui_container = dataMonitor_initUI(tft_Rotation);
        //     break;
        // case dataMonitorChart:
        //     ui_container = dataMonitorChart_initUI(tft_Rotation, highLightChannel);
        //     break;
        // case dataMonitorCount:
        //     ui_container = dataMonitorCount_initUI(tft_Rotation, highLightChannel);
        //     break;
        // default:
        //     ui_container = dataMonitor_initUI(tft_Rotation);
        //     break;
        // }
        xSemaphoreGive(lvglMutex);
    }

    // Initialize with invalid values to force first update
    latestSensorData.channel0.busCurrent = -999.0f;
    latestSensorData.channel1.busCurrent = -999.0f;
    latestSensorData.channel0.busVoltage = -999.0f;
    latestSensorData.channel1.busVoltage = -999.0f;

    // Force a display refresh
    forceUpdate_flag = true;
    functionMode_ChangeRequested = true;
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_disp_t *disp = lv_disp_get_default();
        lv_refr_now(disp);
        xSemaphoreGive(lvglMutex);
    }

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