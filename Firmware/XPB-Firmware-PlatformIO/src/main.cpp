/**
 * XIAO PowerBread - 面包板电源供应器，实时电压/电流/功率监控
 *
 * [INPUT]: boardConfig.h, xpb_display, INA3221Sensor, dialSwitch, sysConfig, lvgl_ui, rtos_Tasks
 * [OUTPUT]: 系统入口 setup()，硬件初始化序列编排，FreeRTOS 四任务创建
 * [POS]: 整个固件的入口点——交响乐的指挥，协调所有模块的初始化顺序和任务启动
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 *
 * https://github.com/nicho810/XIAO-PowerBread
 * Licensed under the MIT License
 */

//=================================================================
// IMPORTANT: Make sure to define the board type in boardConfig.h !!!
//=================================================================

// Board
#include "boardConfig.h"
#include <Arduino.h>

// RTOS Tasks
#include "dialReadTask.h"     // dial read task
#include "serialTask.h"       // serial task
#include "sensorUpdateTask.h" // sensor update task
#include "lvglTask.h"         // LVGL task

// Display subsystem
#include "xpb_display.h"
#include <lvgl.h>

// LVGL UI
lv_obj_t *ui_container = NULL;
#include "lvgl_ui.h"
#include "lvgl_ui_updateFunc.h"
#include "xpb_color_palette.h"

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

ConfigMode configMode;

// Global variables
volatile function_mode current_functionMode = dataMonitor;
volatile bool functionMode_ChangeRequested = true;
volatile bool highLightChannel_ChangeRequested = false;
uint8_t highLightChannel = 0;
uint8_t forceUpdate_flag = 0;

// LCD Rotation variables
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 0; // default rotation.

// Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;
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

// Add this near the top of the file with other declarations
extern SemaphoreHandle_t configStateMutex;

// Semaphores (lvglMutex 由 xpb_display 模块拥有)
extern SemaphoreHandle_t lvglMutex;
SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xMutexBuffer;

// 应用配置数据到运行时变量
static void apply_config(void)
{
    float shuntResistorCHA = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;
    float shuntResistorCHB = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;
    highLightChannel = sysConfig.cfg_data.default_channel;
    current_functionMode = (function_mode)sysConfig.cfg_data.default_mode;
    inaSensor.setParameter(shuntResistorCHA, shuntResistorCHB);
}

void setup(void)
{
    // Serial Init
    Serial.begin(115200);
    Serial.println("==========[XIAO-PowerBread Boot Info]===========");

    // Dial init
    dial.init();

    // INA3221 Init
    if (!inaSensor.begin())
    {
        while (1)
        {
            // Print error message
            Serial.println("INA3221 initialization failed. Please check the wiring and try again.");
            delay(1000);
            // Since not all XIAO boards have built-in LED, so we don't use LED blink here.
            // Need to find another way to indicate the error besides Serial print. Maybe LCD screen?
            // Todo: Add LCD error message
        }
    }

    // Display subsystem init (LCD + LVGL + input + lvglMutex)
    if (!xpb_display_init(tft_Rotation)) {
        Serial.println("Display initialization failed!");
        while(1) delay(100);
    }

    // Create config mode state mutex
    configStateMutex = xSemaphoreCreateMutex();

    // semaphore init
    xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    if (xSemaphore == NULL)
    {
        Serial.println("Error creating semaphore");
        while (1)
            ;
    }


    // Load the config data
    // Option 1: From default config data
    // sysConfig.loadConfig_from(cfg_data_default); 
    // Option 2: from EEPROM
    sysConfig.begin_EEPROM();
    sysConfig.init_EEPROM(0); // 0=no force write, 1=force write
    sysConfig.loadConfig_from_EEPROM();

    // Print all config data
    Serial.println(sysConfig.output_all_config_data_in_String()); 

    // Check if user want to enter the config mode, 2 = The dial is turned down by user when boot up -> Enter the config mode
    if (dial.readDialStatus() == 2)
    {
        configMode.enterConfigMode();
        tmp_cfg_data = sysConfig.cfg_data; // copy cfg_data to tmp_cfg_data for later use(making changes to the config data)
        Serial.println("> Entering config mode.");
    }
    // Create tasks (Create here because we still need it to update the UI even in config mode)
    xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", STACK_SIZE_SENSOR, NULL, 3, xStack_Sensor, &xTaskBuffer_Sensor);
    xLvglTaskHandle = xTaskCreateStatic(lvglTask, "UI_Update", STACK_SIZE_UI, NULL, 4, xStack_UI, &xTaskBuffer_UI);
    xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);
    xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);

    // Other things to do when in config mode
    if (configMode.configState.isActive == true)
    {
        // init the config mode UI
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
        {
            ui_container = xpb_display_create_config_ui(tft_Rotation);
            forceUpdate_flag = 1;
            lv_refr_now(lv_disp_get_default());
            xSemaphoreGive(lvglMutex);
        }

        // Wait for the user to exit the config mode
        while (configMode.configState.isActive)
        {
            vTaskDelay(pdMS_TO_TICKS(500)); // Small delay to avoid busy-looping
        }

        // save the config data to EEPROM
        sysConfig.saveConfig_to_EEPROM(tmp_cfg_data);
        Serial.println("> Config data saved to EEPROM.");
        sysConfig.loadConfig_from_EEPROM(); // load the config data from EEPROM again and apply it to the variables
        Serial.println(sysConfig.output_all_config_data_in_String());
        Serial.flush();

        apply_config();
        configMode.exitConfigMode();
        Serial.println("> Exiting config mode.");
        Serial.flush();
    }
    else
    {
        apply_config();
    }

    // Init the default UI
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        ui_container = xpb_display_create_ui(current_functionMode, tft_Rotation, highLightChannel);

        // Initialize with invalid values to force first update
        latestSensorData.channel0.busCurrent = -999.0f;
        latestSensorData.channel1.busCurrent = -999.0f;
        latestSensorData.channel0.busVoltage = -999.0f;
        latestSensorData.channel1.busVoltage = -999.0f;

        forceUpdate_flag = true;
        functionMode_ChangeRequested = true;

        lv_refr_now(lv_disp_get_default());
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