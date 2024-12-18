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
 * - LVGL: https://github.com/lvgl/lvgl
 * Note: A modified version of Adafruit_ST7735 library is included since v1.1.2 to fit the LCD module used in this project.
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

// Watchdog
// #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
// #include <Adafruit_SleepyDog.h>
// #define WATCHDOG_TIMEOUT 5000  // 5 seconds
// #endif

// LCD
#include <Adafruit_GFX.h>
#include "XPB_ST7735.h"
#include <SPI.h>
#include <lvgl.h>

// LCD Pin Assignments
#if defined(SEEED_XIAO_RP2040)
#define TFT_CS -1  // CS is always connected to ground in this project.
#define TFT_RST 29 // XIAO-D3
#define TFT_DC 4   // XIAO-D9
#define TFT_MOSI 3 // XIAO-D10
#define TFT_SCLK 2 // XIAO-D8
#elif defined(SEEED_XIAO_RP2350)
// Todo
#elif defined(SEEED_XIAO_C3)
// Todo
#elif defined(SEEED_XIAO_S3)
// Todo
#elif defined(SEEED_XIAO_C6)
// Todo
#endif

// LCD declaration
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // The size of the display is 80x160 (Vertical)
#define screen_width 80
#define screen_height 160

// LVGL declaration
static lv_disp_draw_buf_t draw_buf;
const int buf1_size = screen_width * 160;  // Size for each buffer
static lv_color_t buf1[buf1_size];
static lv_color_t buf2[buf1_size];        // Add second buffer

static lv_obj_t *ui_container = NULL;  // Global container for the chart UI


// Display flushing function
void xpb_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    uint16_t *buffer = (uint16_t *)color_p;
    for (uint32_t i = 0; i < w * h; i++)
    {
        tft.pushColor(buffer[i]);
    }
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

// Add this new function for LVGL tick handling
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->state = LV_INDEV_STATE_RELEASED; // No touch input in this example
}

// UIs
#include "ui_style.h"
#include "lvgl_ui.h"
#include "lvgl_ui_updateFunc.h"

// Dial Switch
#include "dialSwitch.h"
DialFunction dial;
volatile int dialStatus = 0; // 0: reset, 1: up, 2: down, 3: press, 4: long press
volatile int lastDialStatus = 0;

// LCD Rotation
volatile bool rotationChangeRequested = false;
volatile int newRotation = 0;
volatile int tft_Rotation = 2; // default rotation.

// Current sensor
#include "INA3221Sensor.h"
INA3221Sensor inaSensor;
DualChannelData latestSensorData; // Add a global variable to store the latest sensor data
DualChannelData oldSensorData;
float avgS[2] = {0}, avgM[2] = {0}, avgA[2] = {0}, peak[2] = {0}; // Average values for each channel
unsigned long dataCount[2] = {0};                                 // Data count for each channel
unsigned long lastUpdate = 0;                                     // Last update time
double sumS[2] = {0}, sumM[2] = {0}, sumA[2] = {0};               // Sum of values for each channel
unsigned long countS[2] = {0}, countM[2] = {0};                   // Count of values for each channel

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
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // Decrease from 20ms to 10ms

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Read sensor data
            DualChannelData newSensorData = inaSensor.readCurrentSensors();

            // Update oldSensorData if values have changed
            if (newSensorData.channel0.isDirty || newSensorData.channel1.isDirty)
            {
                oldSensorData = latestSensorData;
            }

            // Update latestSensorData
            latestSensorData = newSensorData;

            // Take the mutex before updating the chart
            if (xSemaphoreTake(lvglMutex, 0) == pdTRUE) {  // Use 0 timeout for non-blocking
                if (ui_container != NULL && 
                    (newSensorData.channel0.isDirty || newSensorData.channel1.isDirty)) {
                    // Combine all UI updates into a single invalidation cycle
                    lv_obj_invalidate(ui_container);
                    
                    lv_obj_t *monitorContainer = lv_obj_get_child(ui_container, 0);
                    if (monitorContainer != NULL) {
                        update_monitor_data(monitorContainer,1, latestSensorData);
                    }

                    lv_obj_t *chartContainer = lv_obj_get_child(ui_container, 1);
                    if (chartContainer != NULL) {
                        update_chart_data(chartContainer, latestSensorData.channel0.busCurrent);
                    }
                    
                    lv_disp_t * disp = lv_disp_get_default();
                    lv_refr_now(disp);
                }
                xSemaphoreGive(lvglMutex);
            }

            xSemaphoreGive(xSemaphore);
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

// LVGL Task Function
void lvglTask(void *parameter)
{
    while (1)
    {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
        {
            lv_timer_handler();
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

// Serial Print Task
void serialPrintTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastPrintTime = 0;
    const TickType_t xPrintIntervals[] = {
        pdMS_TO_TICKS(1000), // 0 - 1000ms
        pdMS_TO_TICKS(500),  // 1 - 500ms
        pdMS_TO_TICKS(100),  // 2 - 100ms
        pdMS_TO_TICKS(50),   // 3 - 50ms
        pdMS_TO_TICKS(10)    // 4 - 10ms
    };

    // uint8_t intervalIndex = min(sysConfig.cfg_data.serial_printInterval, (uint8_t)4);
    uint8_t intervalIndex = 1;
    const TickType_t xPrintInterval = xPrintIntervals[intervalIndex];
    // const bool serialEnabled = sysConfig.cfg_data.serial_enable;
    const bool serialEnabled = true;
    // const uint8_t serialMode = sysConfig.cfg_data.serial_mode;
    const uint8_t serialMode = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); // Give other tasks a chance to run
        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastPrintTime) >= xPrintInterval)
        {
            if (xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                DualChannelData sensorData = latestSensorData;
                xSemaphoreGive(xSemaphore);

                if (Serial && serialEnabled)
                {
                    char buffer[150]; // Pre-allocate buffer for string formatting
                    if (serialMode == 0)
                    {
                        snprintf(buffer, sizeof(buffer),
                                 "A: %.2fV %.2fmV %.2fmA %.2fmW | B: %.2fV %.2fmV %.2fmA %.2fmW\n",
                                 sensorData.channel0.busVoltage, sensorData.channel0.shuntVoltage * 1000,
                                 sensorData.channel0.busCurrent, sensorData.channel0.busPower,
                                 sensorData.channel1.busVoltage, sensorData.channel1.shuntVoltage * 1000,
                                 sensorData.channel1.busCurrent, sensorData.channel1.busPower);
                    }
                    else
                    {
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

// Dial Read Task
void dialReadTask(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastReadTime = 0;
    const TickType_t xReadInterval = pdMS_TO_TICKS(50); // Read every 50ms
    TickType_t pressStartTime = 0;
    const TickType_t longPressThreshold = pdMS_TO_TICKS(500); // 0.5 second
    bool longPressDetected = false;
    bool shortPressHandled = false;

    while (1)
    {

        TickType_t xCurrentTime = xTaskGetTickCount();
        if ((xCurrentTime - xLastReadTime) >= xReadInterval)
        {
            dialStatus = dial.readDialStatus();

            if (dialStatus == 3)
            { // Pressed
                if (pressStartTime == 0)
                {
                    pressStartTime = xCurrentTime;
                    shortPressHandled = false;
                }
                else if ((xCurrentTime - pressStartTime) >= longPressThreshold && !longPressDetected)
                {
                    longPressDetected = true;
                    Serial.println("Dial long pressed");
                    vTaskDelay(pdMS_TO_TICKS(300)); // Debounce delay
                }
            }
            else if (dialStatus == 0)
            { // Reset
                if (lastDialStatus == 1)
                { // Was Up
                    Serial.println("Dial released from Up");
                    vTaskDelay(pdMS_TO_TICKS(50)); // Debounce delay
                }
                else if (lastDialStatus == 2)
                { // Was Down
                    Serial.println("Dial released from Down");
                    vTaskDelay(pdMS_TO_TICKS(50)); // Debounce delay
                }
                else if (pressStartTime != 0 && !longPressDetected && !shortPressHandled)
                {
                    Serial.println("Dial short pressed");
                    shortPressHandled = true;
                    vTaskDelay(pdMS_TO_TICKS(300)); // Debounce delay
                }
                pressStartTime = 0;
                longPressDetected = false;
            }

            lastDialStatus = dialStatus;
            xLastReadTime = xCurrentTime;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Reduced delay for more frequent checks
    }
}

void setup(void)
{
// SPI Init
#if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
    SPI.setMOSI(TFT_MOSI);
    SPI.setSCK(TFT_SCLK);
    SPI.begin();
#endif

    // Serial Init
    Serial.begin(115200);
    Serial.print(F("Hello! XPB-PIO Test"));

    // Dial init
    dial.init();

    // INA3221 Init
    float shuntResistorCHA = 0.050;
    float shuntResistorCHB = 0.050;
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

    // LCD Init
    tft.setSPISpeed(40000000);  // 40Mhz is the max speed for this display
    tft.initR(INITR_GREENTAB);
    tft.setRotation(tft_Rotation);
    tft.fillScreen(color_Background);

    // LVGL Init
    lv_init();

    // Initialize the display buffer with dual buffering
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf1_size);

    // Initialize display driver with optimized settings
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screen_width;
    disp_drv.ver_res = screen_height;
    disp_drv.flush_cb = xpb_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0;  // Enable partial updates for better performance
    disp_drv.direct_mode = 0;   // Direct mode is not supported
    lv_disp_drv_register(&disp_drv);

    // Set background color
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);

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

    // Create tasks (no watchdog references)
    xSensorTaskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update",
                                          STACK_SIZE_SENSOR, NULL, 3, xStack_Sensor, &xTaskBuffer_Sensor);

    xLvglTaskHandle = xTaskCreateStatic(lvglTask, "UI_Update",
                                      STACK_SIZE_UI, NULL, 3, xStack_UI, &xTaskBuffer_UI);

    xDialTaskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read",
                                        STACK_SIZE_DIAL, NULL, 2, xStack_Dial, &xTaskBuffer_Dial);

    xSerialTaskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print",
                                          STACK_SIZE_SERIAL, NULL, 2, xStack_Serial, &xTaskBuffer_Serial);

    Serial.println("-----------[Boot info end]------------");

    //Call the init LVGL UI
    // lvgl_ui_t1();
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
        lv_obj_clean(lv_scr_act());
        // create_button_widget();
        // dataMonitor_initUI(tft_Rotation);
        // dataMonitorCount_initUI(tft_Rotation, 2);
        ui_container = dataMonitorChart_initUI(tft_Rotation, 2);
        lv_disp_t * disp = lv_disp_get_default();
        lv_refr_now(disp);
        xSemaphoreGive(lvglMutex);
    }

    // Start the scheduler
    // vTaskStartScheduler(); //no need to call this, it will cause a crash
}

// Replace the loop function
void loop()
{
    // Empty - tasks handle everything
}

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
