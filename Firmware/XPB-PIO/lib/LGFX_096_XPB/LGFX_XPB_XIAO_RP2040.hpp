#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

// LGFX for Waveshare RP2040-LCD-0.96
// https://www.waveshare.com/wiki/RP2040-LCD-0.96

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
      cfg.pin_sclk   = 2;
      cfg.pin_miso   = -1;
      cfg.pin_mosi   = 3;
      cfg.pin_dc     = 4;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs       = -1;
      cfg.pin_rst      = 29;
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
