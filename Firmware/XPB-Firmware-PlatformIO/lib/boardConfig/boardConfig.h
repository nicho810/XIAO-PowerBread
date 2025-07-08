#ifndef boardConfig_h
#define boardConfig_h

#if defined(Proj_XIAOPowerBread)

#define screen_width 80
#define screen_height 160

//INA3221
#define currentSensor_TotalChannel 2 //2 channel, only use 2 channel for this project

#define currentSensor_0_address 0x40
#define currentSensor_0_shuntR_0 0.050 //mOhm
#define currentSensor_0_shuntR_1 0.050 //mOhm
#define currentSensor_0_shuntR_2 0.050 //mOhm

/* Pin definition for each board */
#if defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3)
#define pin_dial D2 //D2
#define pin_lcd_sclk D8 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi D10 //D10
#define pin_lcd_dc D9 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst D3 //D3
#define pin_i2c_sda D4 //D4
#define pin_i2c_scl D5 //D5

#elif defined(SEEED_XIAO_ESP32C6)
#define pin_dial D2 //D2
#define pin_lcd_sclk D8 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi D10 //D10
#define pin_lcd_dc D9 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst D3 //D3
#define pin_i2c_sda D4 //D4
#define pin_i2c_scl D5 //D5

/* board = SEEED_XIAO_RP2040 */
#elif defined(SEEED_XIAO_RP2040)
/* board = SEEED_XIAO_RP2040 */
#define pin_dial 28 //D2
#define pin_lcd_sclk 2 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi 3 //D10
#define pin_lcd_dc 4 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst 29 //D3
#define pin_i2c_sda 6 //D4, Usecase: Wire1.setSDA(pin_i2c_sda);
#define pin_i2c_scl 7 //D5, Usecase: Wire1.setSCL(pin_i2c_scl);

#elif defined(SEEED_XIAO_RP2350)
/* board = SEEED_XIAO_RP2350 */
#define pin_dial 28 //D2
#define pin_lcd_sclk 2 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi 3 //D10
#define pin_lcd_dc 4 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst 5 //D3
#define pin_i2c_sda 6 //D4, Usecase: Wire1.setSDA(pin_i2c_sda);
#define pin_i2c_scl 7 //D5, Usecase: Wire1.setSCL(pin_i2c_scl);

#endif
/* End of Pin definition */

/* freeRTS dependency for each board */
#if defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3) || defined(SEEED_XIAO_ESP32C6)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif

/* End of freeRTS dependency */

/* Note:
 * 1. The SPI peripheral(0 or SPI1_HOST) configuration is different for each board. it defined in the LGFX_096_XPB.hpp
 * 2. The I2C peripheral(Wire or Wire1) configuration is different for each board. it defined in the INA3221Sensor.cpp
 */


#endif

#if defined(Proj_XIAOPowerMonitor)

//LCD definition
#define screen_width 135
#define screen_height 240

//INA226 x2
#define currentSensor_TotalChannel 2 //2 channel

#define currentSensor_0_address 0x40
#define currentSensor_0_shuntR_0 0.01f //10 mOhm
#define currentSensor_0_MaxCurrent_0 8.0f //8A

#define currentSensor_1_address 0x41
#define currentSensor_1_shuntR_0 0.01f //10 mOhm
#define currentSensor_1_MaxCurrent_0 8.0f //8A


/* Pin definition for each board */
#if defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3)
#define pin_button D0 //D0

#define pin_dial pin_button //XPM use 3 buttons to replace dial switch.

#define pin_lcd_sclk D8 //D8
#define pin_lcd_miso D9 //Actually not used
#define pin_lcd_mosi D10 //D10
#define pin_lcd_dc D9 //D9
#define pin_lcd_cs D3 //D3
#define pin_lcd_rst D1 //D1

#define pin_sd_sclk D8 //D8
#define pin_sd_miso D9 //D9
#define pin_sd_mosi D10 //D10
#define pin_sd_cs D2 //D2

#define pin_i2c_sda D4 //D4
#define pin_i2c_scl D5 //D5

#elif defined(SEEED_XIAO_ESP32C6)
#define pin_dial D2 //D2
#define pin_lcd_sclk D8 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi D10 //D10
#define pin_lcd_dc D9 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst D3 //D3
#define pin_i2c_sda D4 //D4
#define pin_i2c_scl D5 //D5

/* board = SEEED_XIAO_RP2040 */
#elif defined(SEEED_XIAO_RP2040)
/* board = SEEED_XIAO_RP2040 */
#define pin_dial 28 //D2
#define pin_lcd_sclk 2 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi 3 //D10
#define pin_lcd_dc 4 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst 29 //D3
#define pin_i2c_sda 6 //D4, Usecase: Wire1.setSDA(pin_i2c_sda);
#define pin_i2c_scl 7 //D5, Usecase: Wire1.setSCL(pin_i2c_scl);

#elif defined(SEEED_XIAO_RP2350)
/* board = SEEED_XIAO_RP2350 */
#define pin_dial 28 //D2
#define pin_lcd_sclk 2 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi 3 //D10
#define pin_lcd_dc 4 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst 5 //D3
#define pin_i2c_sda 6 //D4, Usecase: Wire1.setSDA(pin_i2c_sda);
#define pin_i2c_scl 7 //D5, Usecase: Wire1.setSCL(pin_i2c_scl);

#endif
/* End of Pin definition */

/* freeRTS dependency for each board */
#if defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32S3) || defined(SEEED_XIAO_ESP32C6)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#elif defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif

/* End of freeRTS dependency */

/* Note:
 * 1. The SPI peripheral(0 or SPI1_HOST) configuration is different for each board. it defined in the LGFX_096_XPB.hpp
 * 2. The I2C peripheral(Wire or Wire1) configuration is different for each board. it defined in the INA3221Sensor.cpp
 */


#endif

#endif