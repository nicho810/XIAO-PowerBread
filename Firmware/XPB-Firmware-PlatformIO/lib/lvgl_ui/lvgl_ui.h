#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "Arduino.h"
#include "boardConfig.h"
#include "lvgl.h"
#include "lvgl_uiManager.h"
#include "lvgl_ui_mode.h"


#include <LovyanGFX.h>
#if defined(Proj_XIAOPowerBread)
#include <LGFX_096_XPB.hpp>
#elif defined(Proj_XIAOPowerMonitor)
#include <LGFX_114_XPM.hpp>
#endif

#include "lvgl_keyboard.h"


extern LGFX tft; // Extern LGFX instance (will be defined elsewhere)
extern lv_indev_drv_t indev_drv;




void lvgl_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lvgl_init(lv_obj_t *ui_container);



#endif
