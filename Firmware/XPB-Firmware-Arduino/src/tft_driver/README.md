# About this TFT Driver

This TFT driver is a modified version of the Adafruit ST7735 library, specifically adapted for use with the XIAO-PowerBread hardware. 

Due to some init code is different from the original library, which will cause some display issues, In order to fix it and make it work for compiled successfully, I modified some init code and functions, and rename the name of the library to `XPB_ST77xx.h` to avoid conflict with the original library.

## Modifications

1. The driver has been customized to work with the specific TFT LCD used on the XIAO-PowerBread.
2. Some definitions have been changed to fit the XIAO-PowerBread's hardware configuration.
3. The driver is optimized for the display size and characteristics of the XIAO-PowerBread's screen.

## Files

- `XPB_ST77xx.h` and `XPB_ST77xx.cpp`: These files contain the base ST77xx driver code, which has been modified for the XIAO-PowerBread.
- `XPB_ST7735.h`: This file contains specific definitions and configurations for the ST7735 chip used in the XIAO-PowerBread's display.

## Usage

To use this driver in your XIAO-PowerBread project, include the appropriate header files and initialize the display using the provided classes and methods. Refer to the example code in the project for specific implementation details.

## Note

This driver is specifically designed for the XIAO-PowerBread and may not be directly compatible with other hardware configurations. If you're using this driver in a different project, you may need to make additional modifications.


## Credits

This driver is based on the Adafruit ST7735 library. We would like to extend our gratitude to Adafruit for their excellent work on the original library.

/**************************************************************************

  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358

  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802

  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088

  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618


  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

 **************************************************************************/