#ifndef boardConfig_h
#define boardConfig_h


#if defined(SEEED_XIAO_C3) || defined(SEEED_XIAO_S3) || defined(SEEED_XIAO_C6)
#define pin_dial 28

#elif defined(SEEED_XIAO_RP2040)
/* board = SEEED_XIAO_RP2040 */
#define pin_dial D2
#define pin_lcd_sclk D8
#define pin_lcd_miso -1
#define pin_lcd_mosi D10
#define pin_lcd_dc D9
#define pin_lcd_cs -1
#define pin_lcd_rst D3
#define pin_i2c_sda D4
#define pin_i2c_scl D5

#elif defined(SEEED_XIAO_RP2350)
/* board = SEEED_XIAO_RP2350 */
// #define pin_dial D2
// #define pin_lcd_sclk D8
// #define pin_lcd_miso -1
// #define pin_lcd_mosi D10
// #define pin_lcd_dc D9
// #define pin_lcd_cs -1
// #define pin_lcd_rst D3
// #define pin_i2c_sda D4
// #define pin_i2c_scl D5

#define pin_dial 28 //D2
#define pin_lcd_sclk 2 //D8
#define pin_lcd_miso -1 //-1
#define pin_lcd_mosi 3 //D10
#define pin_lcd_dc 4 //D9
#define pin_lcd_cs -1 //-1
#define pin_lcd_rst 5 //D3
#define pin_i2c_sda 6 //D4
#define pin_i2c_scl 7 //D5

#elif defined(RPI_PICO)
/* board = pico */
#define pin_dial 28
#define pin_lcd_sclk 2
#define pin_lcd_miso -1
#define pin_lcd_mosi 3
#define pin_lcd_dc 4
#define pin_lcd_cs -1
#define pin_lcd_rst 29
#define pin_i2c_sda 6 //Usecase: Wire1.setSDA(pin_i2c_sda);
#define pin_i2c_scl 7 //Usecase: Wire1.setSCL(pin_i2c_scl);

#endif

#endif
