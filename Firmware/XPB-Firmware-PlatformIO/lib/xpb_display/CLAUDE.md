# xpb_display/
> L2 | 父级: lib/CLAUDE.md

显示子系统抽象层，封装 LCD 硬件初始化 + LVGL 框架注册 + 输入设备配置。
对外隐藏 LovyanGFX 细节，暴露统一的 UI 创建接口。

## 成员清单

xpb_display.h: 公开接口 — xpb_display_init(), xpb_display_create_ui(), xpb_display_create_config_ui(), XPB_SCREEN_WIDTH/HEIGHT
xpb_display.cpp: 实现 — static LGFX tft (对外隐藏)，LVGL disp_flush/keyboard_read 回调，lvglMutex 定义

## 接口

```cpp
bool xpb_display_init(int rotation);                                      // LCD + LVGL + input + lvglMutex
lv_obj_t* xpb_display_create_ui(function_mode mode, int rotation, uint8_t channel);  // 统一 UI 分发
lv_obj_t* xpb_display_create_config_ui(int rotation);                     // Config mode UI
```

## 依赖

```
xpb_display ← LovyanGFX (LGFX_096_XPB), LVGL, boardConfig, lvgl_ui
xpb_display → lvglMutex (extern 暴露给 rtos_Tasks)
xpb_display ← last_key/last_key_pressed (extern from dialReadTask)
xpb_display ← xSemaphore (extern from main.cpp)
```

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
