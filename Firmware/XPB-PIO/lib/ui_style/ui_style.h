#ifndef UI_STYLE_H
#define UI_STYLE_H

#include <cstdint>


const uint16_t color_Background = 0x0000;
const uint16_t color_Text = 0xFFFF;
const uint16_t color_ChannelA = 0x055B;
const uint16_t color_ChannelB = 0xF97C;
const uint16_t color_GridLines = 0x4208; // Dark gray color, adjust as needed
const uint16_t color_ChartChannelA = 0x02B2;
const uint16_t color_ChartChannelB = 0x5009;

//UI position of the dataMonitor mode, the val might be changed by the rotation
extern int chA_x;
extern int chA_y;
extern int chB_x;
extern int chB_y;



#endif // UI_STYLE_H