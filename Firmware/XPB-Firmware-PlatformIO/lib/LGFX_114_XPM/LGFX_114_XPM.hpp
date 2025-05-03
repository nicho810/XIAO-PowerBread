#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>
#include <boardConfig.h>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;

  public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      #if defined(SEEED_XIAO_RP2040) || defined(SEEED_XIAO_RP2350)
      cfg.spi_host   = 0;
      #elif defined(SEEED_XIAO_ESP32C3) || defined(SEEED_XIAO_ESP32C6)
      cfg.spi_host   = SPI2_HOST;
      #elif defined(SEEED_XIAO_ESP32S3)
      cfg.spi_host   = SPI2_HOST;
      #endif
      cfg.spi_mode   = 0;
      cfg.freq_write = 80000000;
      cfg.pin_sclk   = pin_lcd_sclk;
      cfg.pin_miso   = pin_lcd_miso;
      cfg.pin_mosi   = pin_lcd_mosi;
      cfg.pin_dc     = pin_lcd_dc;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs       = pin_lcd_cs;
      cfg.pin_rst      = pin_lcd_rst;
      cfg.panel_width  = screen_width;
      cfg.panel_height = screen_height;
      cfg.offset_x     = 52;
      cfg.offset_y     = 40;
      cfg.invert       = true;
      cfg.rgb_order    = false;
      cfg.offset_rotation = 0;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl      = -1;
      cfg.pwm_channel = 1;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
};
