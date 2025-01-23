#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>
#include <boardConfig.h>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7735S _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;

  public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host   = 0;
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
      cfg.panel_width  = 80;
      cfg.panel_height = 160;
      cfg.offset_x     = 24;
      cfg.offset_y     = 0;
      cfg.invert       = false;
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
