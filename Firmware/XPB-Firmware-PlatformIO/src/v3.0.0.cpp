#include <Arduino.h>
#include "boardConfig.h"
#include "global_variables.h"

RTOS Tasks
#include "task_serial.h"       // serial task
#include "task_sensorUpdate.h" // sensor update task


// // System Config
// #include "sysConfig.h"
// SysConfig sysConfig;
// sysConfig_data tmp_cfg_data;
// // sysConfig_data cfg_data_default; // default config data


// // Global variables
// ConfigMode configMode;
// SemaphoreHandle_t configStateMutex = NULL;
// float shuntResistorCHA = 0.0f;
// float shuntResistorCHB = 0.0f;
// volatile function_mode current_functionMode = Mode_1;  //mode_1: basic monitor, mode_2: chart, mode_3: count
// volatile bool functionMode_ChangeRequested = true;
// volatile bool highLightChannel_ChangeRequested = false;
// uint8_t highLightChannel = 0;
// uint8_t forceUpdate_flag = 0;



// Current sensor
#if defined(Proj_XIAOPowerBread)
#include "CurrentSensor_INA3221.h"
CurrentSensor_INA3221 c_Sensor;
#elif defined(Proj_XIAOPowerMonitor)
#include "CurrentSensor_INA226.h"
CurrentSensor_DualINA226 c_Sensor;
#endif

// DualChannelData latestSensorData;                                 // Add a global variable to store the latest sensor data
// float avgS[2] = {0}, avgM[2] = {0}, avgH[2] = {0}, peak[2] = {0}; // Average values for each channel

// FreeRTOS Task Declarations
#if defined(SEEED_XIAO_ESP32C3)
// #define STACK_SIZE_UI 8192        // Increased from 4096
#define STACK_SIZE_SERIAL 2048    // Reduced from 4096
// #define STACK_SIZE_DIAL 2048      // Reduced from 4096
#define STACK_SIZE_SENSOR 2048    // Reduced from 4096
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#define STACK_SIZE_UI 2048
#define STACK_SIZE_SERIAL 1024
#define STACK_SIZE_DIAL 1024
#define STACK_SIZE_SENSOR 1024
#endif

// Task buffers and stacks
// StaticTask_t xTaskBuffer_UI, xTaskBuffer_Serial, xTaskBuffer_Dial, xTaskBuffer_Sensor;
// StackType_t xStack_UI[STACK_SIZE_UI];
// StackType_t xStack_Serial[STACK_SIZE_SERIAL];
// StackType_t xStack_Dial[STACK_SIZE_DIAL];
StaticTask_t xTaskBuffer_Serial, xTaskBuffer_Sensor;
StackType_t xStack_Serial[STACK_SIZE_SERIAL];
StackType_t xStack_Sensor[STACK_SIZE_SENSOR];

// Task handles
// TaskHandle_t xLvglTaskHandle = NULL;
TaskHandle_t xSerialTaskHandle = NULL;
// TaskHandle_t xDialTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;

// Other semaphores
// SemaphoreHandle_t lvglMutex = NULL;
SemaphoreHandle_t xSemaphore = NULL;
// StaticSemaphore_t xMutexBuffer_lvgl;
// StaticSemaphore_t xMutexBuffer_config;
// StaticSemaphore_t xMutexBuffer_main;

//Variable for 
// volatile bool configModeExitRequested = false;

// Add a timeout mechanism to the main loop waiting for config mode exit
// unsigned long configModeStartTime = millis();
// const unsigned long CONFIG_MODE_TIMEOUT = 60000; // 1 minute timeout


// Modify your task creation priorities
// #define TASK_PRIORITY_UI_INIT    3  // Reduced from 4
// #define TASK_PRIORITY_LVGL       3  // Keep same as UI init
#define TASK_PRIORITY_SENSOR     2  // Keep as is
#define TASK_PRIORITY_SERIAL     1  // Keep as is

void setup(void)
{
    // Serial Init
    Serial.begin(115200);
    delay(1000);
    Serial.println("==========[Boot Info]===========");

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

    // Initialize all mutexes first
    Serial.println("Initializing mutexes...");
    Serial.flush();

    // Modify mutex creation to include inheritance
    lvglMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer_lvgl);
    configStateMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer_config);
    xSemaphore = xSemaphoreCreateMutexStatic(&xMutexBuffer_main);
    
    if (lvglMutex == NULL || configStateMutex == NULL || xSemaphore == NULL) {
        Serial.println("ERROR: Failed to create mutexes!");
        while(1) delay(100);
    }

    Serial.println("All mutexes initialized successfully");
    Serial.flush();

    // Now proceed with LVGL initialization
    Serial.printf("Free heap before LVGL init: %d\n", ESP.getFreeHeap());
    Serial.printf("Largest free block: %d\n", ESP.getMaxAllocHeap());
    Serial.flush();

    Serial.println("Checking LVGL status...");
    Serial.flush();

    // Initialize LVGL
    lv_init();
    Serial.println("LVGL core initialized");
    Serial.flush();

    // Initialize display buffer with smaller size
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[screen_width * 10];
    static lv_color_t buf2[screen_width * 10];
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_width * 10);
    Serial.println("Display buffer initialized");
    Serial.flush();

    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screen_width;
    disp_drv.ver_res = screen_height;
    disp_drv.flush_cb = lvgl_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    disp_drv.direct_mode = 0;
    disp_drv.antialiasing = 0;

    // Register the driver
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    if (!disp) {
        Serial.println("ERROR: Failed to register display driver!");
        while(1) delay(100);
    }
    Serial.println("Display driver registered");
    Serial.flush();

    // Create a default group for input devices
    lv_group_t * g = lv_group_create();
    if (!g) {
        Serial.println("ERROR: Failed to create input group!");
        while(1) delay(100);
    }
    lv_group_set_default(g);
    Serial.println("Input group created");
    Serial.flush();

    // Now try to initialize the screen
    Serial.println("Initializing screen...");
    Serial.flush();

    // Take the mutex AFTER it's been created
    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        Serial.println("ERROR: Failed to take LVGL mutex!");
        while(1) delay(100);
    }

    // Initialize basic screen
    lv_obj_t * scr = lv_scr_act();
    if (!scr) {
        Serial.println("ERROR: Failed to get active screen!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    // Create a test label
    lv_obj_t * label = lv_label_create(scr);
    if (!label) {
        Serial.println("ERROR: Failed to create test label!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    lv_label_set_text(label, "Initializing...");
    lv_obj_center(label);

    // Force a refresh
    lv_refr_now(disp);

    xSemaphoreGive(lvglMutex);
    Serial.println("Basic UI initialized");
    Serial.flush();

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

    // Load the config data
    // Option 1: From default config data
    // sysConfig.loadConfig_from(cfg_data_default); 
    // Option 2: from EEPROM
    sysConfig.init_EEPROM(0); // 0=no force write, 1=force write
    sysConfig.loadConfig_from_EEPROM();

    // Print all config data
    Serial.println(sysConfig.output_all_config_data_in_String()); 

    // Create tasks with proper return type checking
    TaskHandle_t taskHandle;

    // Create sensor task first as it's highest priority
    taskHandle = xTaskCreateStatic(sensorUpdateTask, "Sensor_Update", 
        STACK_SIZE_SENSOR, NULL, TASK_PRIORITY_SENSOR, 
        xStack_Sensor, &xTaskBuffer_Sensor);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create sensor task!");
        while(1) delay(100);
    }
    Serial.println("> Sensor task created successfully");

    // Create LVGL task
    taskHandle = xTaskCreateStatic(lvglTask, "UI_Update", 
        STACK_SIZE_UI, NULL, TASK_PRIORITY_LVGL, 
        xStack_UI, &xTaskBuffer_UI);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create LVGL task!");
        while(1) delay(100);
    }
    Serial.println("> LVGL task created successfully");

    // Create dial task
    taskHandle = xTaskCreateStatic(dialReadTask, "Dial_Read", 
        STACK_SIZE_DIAL, NULL, 2, // Use numeric priority since TASK_PRIORITY_DIAL is undefined
        xStack_Dial, &xTaskBuffer_Dial);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create dial task!");
        while(1) delay(100);
    }
    Serial.println("> Dial task created successfully");

    // Create serial task
    taskHandle = xTaskCreateStatic(serialPrintTask, "Serial_Print", 
        STACK_SIZE_SERIAL, NULL, TASK_PRIORITY_SERIAL, 
        xStack_Serial, &xTaskBuffer_Serial);
    if (taskHandle == nullptr) {
        Serial.println("ERROR: Failed to create serial task!");
        while(1) delay(100);
    }
    Serial.println("> Serial task created successfully");

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

    Serial.println("Config data applied.");
    Serial.flush();

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

    Serial.println("Initializing UI...");
    Serial.flush();

    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        Serial.println("ERROR: Failed to take LVGL mutex for UI init!");
        while(1) delay(100);
    }

    // Make the base container static to prevent deallocation
    static lv_obj_t *base_container = NULL;

    Serial.println("Cleaning screen...");
    Serial.flush();
    lv_obj_clean(lv_scr_act());

    Serial.println("Creating base container...");
    Serial.flush();

    // Create a base container with minimal styling first
    base_container = lv_obj_create(lv_scr_act());
    if (!base_container || !lv_obj_is_valid(base_container)) {
        Serial.println("ERROR: Failed to create base container!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    // Set basic properties
    Serial.println("Setting container properties...");
    Serial.flush();

    lv_obj_set_size(base_container, screen_width, screen_height);
    lv_obj_center(base_container);
    lv_obj_clear_flag(base_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(base_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(base_container, 0, LV_PART_MAIN);

    // Force a refresh to ensure container is created
    lv_refr_now(lv_disp_get_default());

    Serial.println("Basic container created successfully");
    Serial.flush();

    // Now try to initialize the actual UI
    Serial.println("Initializing UI container...");
    Serial.flush();

    // Store the current mode for debugging
    Serial.printf("Current function mode: %d\n", (int)current_functionMode);
    Serial.flush();

    // Verify container is still valid
    if (!lv_obj_is_valid(base_container)) {
        Serial.println("ERROR: Base container became invalid!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    try {
        // Initialize UI based on mode
        switch (current_functionMode) {
            case Mode_1:
                Serial.println("Initializing Mode 1 (Basic Monitor)");
                dataMonitor_initUI(base_container, highLightChannel, latestSensorData, 0.0f, 0);
                break;
                
            case Mode_2:
                Serial.println("Initializing Mode 2 (Chart)");
                dataMonitorChart_initUI(base_container, highLightChannel, latestSensorData, 0.0f, 0);
                break;
                
            case Mode_3:
                Serial.println("Initializing Mode 3 (Count)");
                dataMonitorCount_initUI(base_container, highLightChannel, latestSensorData, 0.0f, 0);
                break;
                
            default:
                Serial.println("ERROR: Invalid function mode!");
                xSemaphoreGive(lvglMutex);
                while(1) delay(100);
        }
    } catch (...) {
        Serial.println("ERROR: Exception during UI initialization!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    // Verify container one last time
    if (!lv_obj_is_valid(base_container)) {
        Serial.println("ERROR: Base container invalid after UI init!");
        xSemaphoreGive(lvglMutex);
        while(1) delay(100);
    }

    Serial.println("UI initialization complete");
    Serial.flush();
    xSemaphoreGive(lvglMutex);

    // Initialize with invalid values to force first update
    latestSensorData.channel0.busCurrent = -999.0f;
    latestSensorData.channel1.busCurrent = -999.0f;
    latestSensorData.channel0.busVoltage = -999.0f;
    latestSensorData.channel1.busVoltage = -999.0f;

    // Force a display refresh
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