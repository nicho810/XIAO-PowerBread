/**
 * [INPUT]: lvgl.h (lv_color_t 类型)
 * [OUTPUT]: 7 个 extern lv_color_t 颜色常量 (Background/Text/ChannelA/ChannelB/GridLines + 暗色变体)
 * [POS]: UI 主题颜色单一真相源，双通道色彩体系 (cyan/magenta)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef XPB_COLOR_PALETTE_H
#define XPB_COLOR_PALETTE_H

#include "lvgl.h"

extern lv_color_t xpb_color_Background;
extern lv_color_t xpb_color_Text;
extern lv_color_t xpb_color_ChannelA;
extern lv_color_t xpb_color_ChannelB;
extern lv_color_t xpb_color_GridLines;
extern lv_color_t xpb_color_ChannelA_dark;
extern lv_color_t xpb_color_ChannelB_dark;

#endif


/*
const uint16_t color_Background = 0x0000;
const uint16_t color_Text = 0xFFFF;
const uint16_t color_ChannelA = 0x055B;
const uint16_t color_ChannelB = 0xF97C;
const uint16_t color_GridLines = 0x4208; // Dark gray color, adjust as needed
const uint16_t color_ChartChannelA = 0x02B2;
const uint16_t color_ChartChannelB = 0x5009;
*/