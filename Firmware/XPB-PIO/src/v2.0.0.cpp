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

// RTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "dialReadTask.h"
#include "serialTask.h"

// LCD
#include <LovyanGFX.h>
#include <LGFX_XPB_XIAO_RP2040.hpp> //only for RP2040
#include <lvgl.h>

static LGFX tft;                 // LGFXのインスタンスを作成。
static LGFX_Sprite sprite(&tft); // スプライトを使う場合はLGFX_Spriteのインスタンスを作成。

#define screen_width 80
#define screen_height 160

static lv_obj_t *ui_container = NULL; // Global container for the chart UI

// LVGL Input Device Declaration
static lv_indev_drv_t indev_drv;

// keyboard read function
static void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    if (last_key_pressed)
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = last_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
        data->key = last_key; // Pass the last key that was pressed
    }

    // Reset the key press state after it's been read
    last_key_pressed = false;
}


// Display flushing function
void xpb_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t len = w * h;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    // Replace individual pushColor with bulk writePixels
    tft.writePixels((uint16_t *)&color_p->full, len, true);

    tft.endWrite();
    lv_disp_flush_ready(disp);
}

// UIs
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
sysConfig_data cfg_data_default; // default config data

// Function Mode
enum function_mode
{
    dataMonitor,
    dataMonitorChart,
    dataMonitorCount,
    // serialMonitor,
    // pwmOutput,
    // analogInputMonitor,
};
volatile function_mode current_functionMode = dataMonitor;
volatile bool functionMode_ChangeRequested = true; // a flag to indicate a mode change is requested
volatile bool highLightChannel_ChangeRequested = false;
uint8_t highLightChannel = 0;
uint8_t forceUpdate_flag = 0; // 0=no force update, 1=force update

// LCD Rotation
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 0; // default rotation.

// Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;
DualChannelData latestSensorData;                                 // Add a global variable to store the latest sensor data
float avgS[2] = {0}, avgM[2] = {0}, avgH[2] = {0}, peak[2] = {0}; // Average values for each channel

// FreeRTOS Task Declarations
#if defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C6)
#define STACK_SIZE_UI 4096
#define STACK_SIZE_SERIAL 4096
#define STACK_SIZE_DIAL 2048
#define STACK_SIZE_SENSOR 2048
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
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

TaskHandle_t xLvglTaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
TaskHandle_t xDialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;

SemaphoreHandle_t lvglMutex = NULL;
SemaphoreHandle_t xSemaphore = NULL;

StaticSemaphore_t xMutexBuffer;

// Sensor Update Task
void sensorUpdateTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10);
    const float UPDATE_THRESHOLD = 0.005f; // 5mV/mA threshold

    // Static buffers for averaging
    static const int SHORT_SAMPLES = 100; // 1 second
    static const int MED_SAMPLES = 1000;  // 10 seconds
    static const int LONG_SAMPLES = 6000; // 60 seconds
    static float shortBuffer[2][SHORT_SAMPLES] = {{0}};
    static float medBuffer[2][MED_SAMPLES] = {{0}};
    static float longBuffer[2][LONG_SAMPLES] = {{0}};
    static int shortIndex = 0;
    static int medIndex = 0;
    static int longIndex = 0;

    // Add these static variables at the start of sensorUpdateTask
    static bool buffersInitialized = false;
    static int sampleCount[2] = {0, 0};

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            DualChannelData newSensorData = inaSensor.readCurrentSensors();

            // Calculate averages for dataMonitorCount mode
            if (current_functionMode == dataMonitorCount)
            {
                for (int ch = 0; ch < 2; ch++)
                {
                    float currentVal = (ch == 0) ? newSensorData.channel0.busCurrent
                                                 : newSensorData.channel1.busCurrent;

                    // 1-second moving average
                    float alphaS = 0.3f; // Faster response time
                    avgS[ch] = (alphaS * currentVal) + ((1.0f - alphaS) * avgS[ch]);

                    // 1-minute moving average
                    float alphaM = 0.002f;
                    avgM[ch] = (alphaM * currentVal) + ((1.0f - alphaM) * avgM[ch]);

                    // 1-hour moving average
                    float alphaH = 0.000033f;
                    avgH[ch] = (alphaH * currentVal) + ((1.0f - alphaH) * avgH[ch]);

                    // Update peak values
                    if (currentVal > peak[ch])
                    {
                        peak[ch] = currentVal;
                    }
                }

                // Update buffer indices
                shortIndex = (shortIndex + 1) % SHORT_SAMPLES;
                medIndex = (medIndex + 1) % MED_SAMPLES;
                longIndex = (longIndex + 1) % LONG_SAMPLES;
            }

            // Check if display update is needed
            bool shouldUpdate = forceUpdate_flag;
            if (!shouldUpdate)
            {
                shouldUpdate = (abs(newSensorData.channel0.busCurrent - latestSensorData.channel0.busCurrent) > UPDATE_THRESHOLD) ||
                               (abs(newSensorData.channel1.busCurrent - latestSensorData.channel1.busCurrent) > UPDATE_THRESHOLD) ||
                               (abs(newSensorData.channel0.busVoltage - latestSensorData.channel0.busVoltage) > UPDATE_THRESHOLD) ||
                               (abs(newSensorData.channel1.busVoltage - latestSensorData.channel1.busVoltage) > UPDATE_THRESHOLD);
            }

            if (shouldUpdate)
            {
                latestSensorData = newSensorData;

                if (xSemaphoreTake(lvglMutex, 0) == pdTRUE)
                {
                    lv_obj_t *container0 = lv_obj_get_child(ui_container, 0);

                    if (container0)
                    { // At least one container exists
                        switch (current_functionMode)
                        {
                        case dataMonitor:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, 0, latestSensorData);
                                update_monitor_data(container1, 1, latestSensorData);
                            }
                            break;

                        case dataMonitorChart:
                            if (lv_obj_t *container1 = lv_obj_get_child(ui_container, 1))
                            {
                                update_monitor_data(container0, highLightChannel, latestSensorData);
                                if (highLightChannel == 0)
                                {
                                    update_chart_data(container1, latestSensorData.channel0.busCurrent);
                                }
                                else
                                {
                                    update_chart_data(container1, latestSensorData.channel1.busCurrent);
                                }
                            }
                            break;

                        case dataMonitorCount:
                            update_monitor_data(container0, 0, latestSensorData);
                            lv_obj_t *container1 = lv_obj_get_child(ui_container, 1);
                            update_count_data(container1, highLightChannel, avgS[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 2);
                            update_count_data(container1, highLightChannel, avgM[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 3);
                            update_count_data(container1, highLightChannel, avgH[highLightChannel]);
                            container1 = lv_obj_get_child(ui_container, 4);
                            update_count_data(container1, highLightChannel, peak[highLightChannel]);
                            break;
                        }

                        // lv_timer_handler(); //Note: no need to call here, let lvgl_task handle it
                    }
                    xSemaphoreGive(lvglMutex);
                }
            }

            xSemaphoreGive(xSemaphore);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

// LVGL Task Function
void lvglTask(void *parameter)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(5); // Increase refresh rate to 200Hz

    while (1)
    {
        if (xSemaphoreTake(lvglMutex, 0) == pdTRUE)
        { // Non-blocking mutex take
            lv_timer_handler();
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(xFrequency);
    }
}

// Serial Print Task
// void serialPrintTask(void *pvParameters)
// {
//     (void)pvParameters;
//     TickType_t xLastPrintTime = 0;
//     const TickType_t xPrintIntervals[] = {
//         pdMS_TO_TICKS(1000), // 0 - 1000ms
//         pdMS_TO_TICKS(500),  // 1 - 500ms
//         pdMS_TO_TICKS(100),  // 2 - 100ms
//         pdMS_TO_TICKS(50),   // 3 - 50ms
//         pdMS_TO_TICKS(10)    // 4 - 10ms
//     };
//
//     // uint8_t intervalIndex = min(sysConfig.cfg_data.serial_printInterval, (uint8_t)4);
//     uint8_t intervalIndex = 0;
//     const TickType_t xPrintInterval = xPrintIntervals[intervalIndex];
//     // const bool serialEnabled = sysConfig.cfg_data.serial_enable;
//     const bool serialEnabled = true;
//     // const uint8_t serialMode = sysConfig.cfg_data.serial_mode;
//     const uint8_t serialMode = 0;
//
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(50)); // Give other tasks a chance to run
//         TickType_t xCurrentTime = xTaskGetTickCount();
//         if ((xCurrentTime - xLastPrintTime) >= xPrintInterval)
//         {
//             if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(100)) == pdTRUE)
//             {
//                 DualChannelData sensorData = latestSensorData;
//                 xSemaphoreGive(xSemaphore);
//
//                 if (Serial && serialEnabled)
//                 {
//                     char buffer[150]; // Pre-allocate buffer for string formatting
//                     if (serialMode == 0)
//                     {
//                         snprintf(buffer, sizeof(buffer),
//                                  "A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
//                                  sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000,
//                                  sensorData.channel0.busCurrent, sensorData.channel0.busPower,
//                                  sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000,
//                                  sensorData.channel1.busCurrent, sensorData.channel1.busPower);
//                     }
//                     else
//                     {
//                         snprintf(buffer, sizeof(buffer),
//                                  "V0:%.2f,I0:%.2f,P0:%.2f,V1:%.2f,I1:%.2f,P1:%.2f\n",
//                                  sensorData.channel0.busVoltage, sensorData.channel0.busCurrent,
//                                  sensorData.channel0.busPower, sensorData.channel1.busVoltage,
//                                  sensorData.channel1.busCurrent, sensorData.channel1.busPower);
//                     }
//                     Serial.print(buffer);
//                 }
//                 xLastPrintTime = xCurrentTime;
//             }
//         }
//     }
// }



void setup(void)
{
    // Serial Init
    Serial.begin(115200);
    Serial.print(F("Hello! XPB-PIO Test"));

    // Dial init
    dial.init();

    // load default config data
    sysConfig.loadConfig_from(cfg_data_default); // load default config data
    // Load config from EEPROM
    // sysConfig.begin_EEPROM();
    // sysConfig.init_EEPROM(0); //0=no force write, 1=force write
    Serial.println(sysConfig.output_all_config_data_in_String()); // print all config data

    // Apply the cfg_data to the variables
    float shuntResistorCHA = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;
    float shuntResistorCHB = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;
    highLightChannel = sysConfig.cfg_data.default_channel;                 // 0=channel A, 1=channel B, it used when only show one channel data
    current_functionMode = (function_mode)sysConfig.cfg_data.default_mode; // 0=dataMonitor, 1=dataMonitorChart, 2=dataMonitorCount, 3=dataChart
    // current_functionMode = (function_mode)1;

    // INA3221 Init
    if (!inaSensor.begin(shuntResistorCHA, shuntResistorCHB))
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

    tft.init();
    tft.setColorDepth(16);         // RGB565
    tft.setRotation(tft_Rotation); // Set initial hardware rotation = 0
    tft.fillScreen(0x0000); // Black screen

    // LVGL LCD Init
    lv_init();
    // LVGL declaration
    static lv_disp_draw_buf_t draw_buf;
    const int buf1_size = screen_width * 80; // Screen buffer
    static lv_color_t buf1[buf1_size];
    static lv_color_t buf2[buf1_size];                       // Add second buffer
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf1_size); // Initialize the display buffer with dual buffering, decrease the buffer if want to save memory.
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screen_width;
    disp_drv.ver_res = screen_height;
    disp_drv.flush_cb = xpb_disp_flush; // attach the flush callback function
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0; // 0=Enable partial updates for better performance
    disp_drv.direct_mode = 0;  // Direct mode is not supported, set to 0
    disp_drv.antialiasing = 1; // Enable antialiasing for better quality but slower performance (just a bit?)
    lv_disp_drv_register(&disp_drv);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);

    // LVGL Input Device Declaration
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv);

    // Set it as default input device (optional)
    if (kb_indev)
    {
        lv_group_t *g = lv_group_create();
        lv_group_set_default(g);
        lv_indev_set_group(kb_indev, g);
    }

    // Create LVGL mutex
    lvglMutex = xSemaphoreCreateMutex();

    // semaphore init
    xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer);
    if (xSemaphore == NULL)
    {
        Serial.println("Error creating semaphore");
        while (1)
            ;
    }

    // Create tasks with optimized priorities
    xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", STACK_SIZE_SENSOR, NULL, 3, xStack_Sensor, &xTaskBuffer_Sensor);
    xLvglTaskHandle = xTaskCreateStatic(lvglTask, "UI_Update", STACK_SIZE_UI, NULL, 4, xStack_UI, &xTaskBuffer_UI);
    xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);
    xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", STACK_SIZE_SERIAL, NULL, 1, xStack_Serial, &xTaskBuffer_Serial);

    Serial.println("-----------[Boot info end]------------");

    // Init the default UI
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_obj_clean(lv_scr_act());
        switch (current_functionMode)
        {
        case dataMonitor:
            ui_container = dataMonitor_initUI(tft_Rotation);
            break;
        case dataMonitorChart:
            ui_container = dataMonitorChart_initUI(tft_Rotation, highLightChannel);
            break;
        case dataMonitorCount:
            ui_container = dataMonitorCount_initUI(tft_Rotation, highLightChannel);
            break;
        default:
            ui_container = dataMonitor_initUI(tft_Rotation);
            break;
        }
        forceUpdate_flag = 1; // set flag to force update to flush the UI

        lv_disp_t *disp = lv_disp_get_default();
        lv_refr_now(disp);
        xSemaphoreGive(lvglMutex);
    }

    // Add LVGL optimization settings
    lv_disp_t *disp = lv_disp_get_default();
    disp->driver->monitor_cb = NULL; // Disable monitor callback

    // Start the scheduler
    // vTaskStartScheduler(); //Note: no need to call this, it will cause a crash
}

// Replace the loop function
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

extern "C" void vApplicationIdleHook(void)
{
    // it use for checking task states when debugging
}

void vApplicationTickHook(void)
{
    // it use for checking task states when debugging
}
