#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include "XPB_ST7735.h"
#include <SPI.h>
#include <lvgl.h> // Include the LVGL library
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Pin Assignments, For RP2040 only
#define TFT_CS -1
#define TFT_RST 29 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 4
#define TFT_MOSI 3 // Data out
#define TFT_SCLK 2 // Clock out

// LCD declaration
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // The size of the display is 80x160 (Vertical)
#define screen_width 80
#define screen_height 160
#define screen_rotation 2

// LVGL declaration
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screen_width * 8];  // Single buffer

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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

// Test code for LCD
void test_lcd()
{
  tft.fillScreen(ST77XX_BLACK); // Add this line - clear the screen
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.print("Hello, World!");
}

// Add these global variables at the top with other declarations
static lv_obj_t *counter_label;
static int counter = 0;

// Add these declarations
static TaskHandle_t lvglTaskHandle = NULL;
static TaskHandle_t counterTaskHandle = NULL;
static TaskHandle_t debugTaskHandle = NULL;
SemaphoreHandle_t lvglMutex = NULL;

// Add LVGL task function
void lvglTask(void *parameter) {
    while (1) {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
            lv_timer_handler();
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

// Add counter task function
void counterTask(void *parameter) {
    while (1) {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
            counter++;
            char buf[32];
            snprintf(buf, sizeof(buf), "Count: %d", counter);
            lv_label_set_text(counter_label, buf);
            lv_obj_invalidate(counter_label);
            
            // Force a display update
            lv_disp_t * disp = lv_disp_get_default();
            lv_refr_now(disp);
            
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Increased delay to 100ms
    }
}

// Add debug task function
void debugTask(void *parameter) {
    while (1) {
        if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
            static int last_counter = 0;
            if (counter != last_counter) {
                Serial.print("Counter: ");
                Serial.println(counter);
                last_counter = counter;
            }
            xSemaphoreGive(lvglMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Print every 5 seconds
    }
}

// Modify setup function
void setup(void) {

  SPI.setMOSI(TFT_MOSI);
  SPI.setSCK(TFT_SCLK);
  SPI.begin();

  Serial.begin(115200);
  Serial.print(F("Hello! ST77xx TFT Test"));

  tft.initR(INITR_GREENTAB);
  tft.setRotation(screen_rotation);
  tft.fillScreen(ST77XX_BLACK); // Add this line - clear the screen

  // test_lcd();

  Serial.println(F("Initialized"));

  lv_init();

  // Initialize the display buffer with single buffering
  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screen_width * 8);

  // Initialize display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screen_width;
  disp_drv.ver_res = screen_height;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.full_refresh = 0;  // Disable full refresh
  lv_disp_drv_register(&disp_drv);

  // Set up label with optimized settings
  counter_label = lv_label_create(lv_scr_act());
  lv_label_set_text(counter_label, "0");
  lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(counter_label, lv_color_make(255, 0, 0), LV_PART_MAIN);

  // Set background color
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);

  // Create LVGL mutex
  lvglMutex = xSemaphoreCreateMutex();
  
  // Create tasks with optimized priorities
  xTaskCreate(
      lvglTask,
      "LVGL Task",
      2048,    // Stack size
      NULL,
      3,       // Highest priority
      &lvglTaskHandle
  );

  xTaskCreate(
      counterTask,
      "Counter Task",
      1024,    // Stack size
      NULL,
      2,       // Medium priority
      &counterTaskHandle
  );

  xTaskCreate(
      debugTask,
      "Debug Task",
      1024,    // Stack size
      NULL,
      1,       // Lowest priority
      &debugTaskHandle
  );

  // Start the scheduler
  // vTaskStartScheduler(); //no need to call this, it will cause a crash
}

// Replace the loop function
void loop() {
    // Empty - tasks handle everything
}
