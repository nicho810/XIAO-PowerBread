/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --bpp 1 --size 8 --no-compress --font Inter_18pt-Bold.ttf --range 32-127 --format lvgl -o inter_bold_8px_basicLatin_1b.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef INTER_BOLD_8PX_BASICLATIN_1B
#define INTER_BOLD_8PX_BASICLATIN_1B 1
#endif

#if INTER_BOLD_8PX_BASICLATIN_1B

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf2,

    /* U+0022 "\"" */
    0xb4,

    /* U+0023 "#" */
    0x52, 0xbe, 0xaf, 0xa9, 0x40,

    /* U+0024 "$" */
    0x23, 0xbd, 0xc7, 0x1a, 0xce, 0x20,

    /* U+0025 "%" */
    0xe5, 0x52, 0xc7, 0x82, 0xe5, 0x53, 0x80,

    /* U+0026 "&" */
    0x63, 0x18, 0x8b, 0x49, 0xe0,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x6a, 0xa9,

    /* U+0029 ")" */
    0x95, 0x56,

    /* U+002A "*" */
    0x5f, 0xa0,

    /* U+002B "+" */
    0x21, 0x3e, 0x42, 0x0,

    /* U+002C "," */
    0x60,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x25, 0x24, 0xa4,

    /* U+0030 "0" */
    0x76, 0xe3, 0x18, 0xed, 0xc0,

    /* U+0031 "1" */
    0x7c, 0x92, 0x48,

    /* U+0032 "2" */
    0x69, 0x13, 0x64, 0xf0,

    /* U+0033 "3" */
    0x69, 0x16, 0x19, 0x60,

    /* U+0034 "4" */
    0x31, 0x94, 0xa9, 0x7c, 0x40,

    /* U+0035 "5" */
    0xf8, 0xe1, 0x19, 0x60,

    /* U+0036 "6" */
    0x69, 0x8e, 0x99, 0x60,

    /* U+0037 "7" */
    0xf1, 0x22, 0x64, 0x40,

    /* U+0038 "8" */
    0x6d, 0xd6, 0x99, 0x60,

    /* U+0039 "9" */
    0x69, 0x99, 0x79, 0x60,

    /* U+003A ":" */
    0x88,

    /* U+003B ";" */
    0x86,

    /* U+003C "<" */
    0x16, 0xc7, 0x10,

    /* U+003D "=" */
    0xf0, 0xf0,

    /* U+003E ">" */
    0x86, 0x3e, 0x80,

    /* U+003F "?" */
    0x6a, 0x26, 0x40, 0x40,

    /* U+0040 "@" */
    0x38, 0x8a, 0x7d, 0x3a, 0x74, 0xe7, 0xa0, 0x3c,

    /* U+0041 "A" */
    0x22, 0x94, 0xaf, 0xc6, 0x20,

    /* U+0042 "B" */
    0xf9, 0x9e, 0x99, 0xf0,

    /* U+0043 "C" */
    0x76, 0xe1, 0x8, 0x6d, 0xc0,

    /* U+0044 "D" */
    0xf4, 0xe3, 0x18, 0xcf, 0xc0,

    /* U+0045 "E" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0046 "F" */
    0xf8, 0x8e, 0x88, 0x80,

    /* U+0047 "G" */
    0x73, 0x28, 0x27, 0x8b, 0x27, 0x0,

    /* U+0048 "H" */
    0x8c, 0x63, 0xf8, 0xc6, 0x20,

    /* U+0049 "I" */
    0xfe,

    /* U+004A "J" */
    0x11, 0x11, 0x19, 0x60,

    /* U+004B "K" */
    0x95, 0xa9, 0xce, 0x4a, 0x60,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0xcf, 0x3c, 0xff, 0xb6, 0xdb, 0x40,

    /* U+004E "N" */
    0x8e, 0x7b, 0x5b, 0xce, 0x20,

    /* U+004F "O" */
    0x76, 0xe3, 0x18, 0xed, 0xc0,

    /* U+0050 "P" */
    0xe9, 0x99, 0xe8, 0x80,

    /* U+0051 "Q" */
    0x76, 0xe3, 0x18, 0xed, 0xc1,

    /* U+0052 "R" */
    0xe9, 0x99, 0xea, 0x90,

    /* U+0053 "S" */
    0x74, 0xb0, 0xe1, 0x49, 0xc0,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x80,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc5, 0xc0,

    /* U+0056 "V" */
    0xcd, 0x24, 0x96, 0x70, 0xc3, 0x0,

    /* U+0057 "W" */
    0xd9, 0xdb, 0x5b, 0x5a, 0x56, 0x66, 0x26,

    /* U+0058 "X" */
    0x9a, 0x9c, 0x47, 0x6a, 0x60,

    /* U+0059 "Y" */
    0x8a, 0x94, 0x42, 0x10, 0x80,

    /* U+005A "Z" */
    0xf3, 0x26, 0x4c, 0xf0,

    /* U+005B "[" */
    0xea, 0xab,

    /* U+005C "\\" */
    0x91, 0x24, 0x89,

    /* U+005D "]" */
    0xd5, 0x57,

    /* U+005E "^" */
    0x46, 0x6a,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x80,

    /* U+0061 "a" */
    0x71, 0xf9, 0xf0,

    /* U+0062 "b" */
    0x88, 0xe9, 0x99, 0xe0,

    /* U+0063 "c" */
    0x69, 0x89, 0x60,

    /* U+0064 "d" */
    0x11, 0x79, 0x99, 0x70,

    /* U+0065 "e" */
    0x69, 0xf8, 0x60,

    /* U+0066 "f" */
    0x6b, 0xa4, 0x90,

    /* U+0067 "g" */
    0x79, 0x99, 0x71, 0x60,

    /* U+0068 "h" */
    0x88, 0xf9, 0x99, 0x90,

    /* U+0069 "i" */
    0xc5, 0x54,

    /* U+006A "j" */
    0xc5, 0x55, 0xc0,

    /* U+006B "k" */
    0x88, 0xae, 0xca, 0xb0,

    /* U+006C "l" */
    0xfe,

    /* U+006D "m" */
    0xff, 0x26, 0x4c, 0x99, 0x20,

    /* U+006E "n" */
    0xf9, 0x99, 0x90,

    /* U+006F "o" */
    0x69, 0x99, 0x60,

    /* U+0070 "p" */
    0xe9, 0x99, 0xe8, 0x80,

    /* U+0071 "q" */
    0x79, 0x99, 0x71, 0x10,

    /* U+0072 "r" */
    0xea, 0x80,

    /* U+0073 "s" */
    0xe8, 0xf9, 0xf0,

    /* U+0074 "t" */
    0x5d, 0x24, 0xc0,

    /* U+0075 "u" */
    0x99, 0x99, 0xf0,

    /* U+0076 "v" */
    0x92, 0x94, 0xe2, 0x0,

    /* U+0077 "w" */
    0x97, 0xa9, 0xd3, 0x66, 0x80,

    /* U+0078 "x" */
    0xd7, 0x27, 0xd0,

    /* U+0079 "y" */
    0x92, 0x94, 0xe2, 0x11, 0x0,

    /* U+007A "z" */
    0xf3, 0x64, 0xf0,

    /* U+007B "{" */
    0x69, 0x2c, 0x93,

    /* U+007C "|" */
    0xff, 0xe0,

    /* U+007D "}" */
    0xc9, 0x26, 0x96,

    /* U+007E "~" */
    0xdb
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 30, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 41, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 68, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 3, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 8, .adv_w = 84, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 14, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 21, .adv_w = 85, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 41, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 27, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 29, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 31, .adv_w = 71, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 33, .adv_w = 86, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 40, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 38, .adv_w = 59, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 39, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 43, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 54, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 82, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 79, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 82, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 74, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 82, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 82, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 40, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 41, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 86, .adv_w = 86, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 86, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 91, .adv_w = 86, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 72, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 106, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 94, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 92, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 78, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 75, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 35, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 74, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 91, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 72, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 97, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 98, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 83, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 99, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 183, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 192, .adv_w = 85, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 197, .adv_w = 93, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 94, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 208, .adv_w = 132, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 93, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 92, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 231, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 234, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 236, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 238, .adv_w = 61, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 239, .adv_w = 45, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 240, .adv_w = 74, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 74, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 75, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 50, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 264, .adv_w = 79, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 34, .box_w = 2, .box_h = 9, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 273, .adv_w = 73, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 34, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 116, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 283, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 78, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 293, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 297, .adv_w = 51, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 71, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 47, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 312, .adv_w = 108, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 73, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 76, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 325, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 59, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 331, .adv_w = 47, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 333, .adv_w = 59, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 336, .adv_w = 86, .box_w = 4, .box_h = 2, .ofs_x = 1, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    3, 7,
    3, 13,
    3, 15,
    3, 21,
    5, 24,
    7, 3,
    7, 8,
    7, 61,
    8, 7,
    8, 13,
    8, 15,
    8, 21,
    9, 93,
    11, 7,
    11, 13,
    11, 15,
    11, 21,
    11, 33,
    11, 64,
    12, 19,
    12, 20,
    12, 24,
    12, 61,
    13, 1,
    13, 2,
    13, 3,
    13, 8,
    13, 17,
    13, 18,
    13, 19,
    13, 20,
    13, 22,
    13, 23,
    13, 24,
    13, 25,
    13, 26,
    13, 32,
    13, 33,
    14, 19,
    14, 20,
    14, 24,
    14, 61,
    15, 1,
    15, 2,
    15, 3,
    15, 8,
    15, 17,
    15, 18,
    15, 19,
    15, 20,
    15, 22,
    15, 23,
    15, 24,
    15, 25,
    15, 26,
    15, 32,
    15, 33,
    16, 13,
    16, 15,
    16, 61,
    17, 13,
    17, 15,
    17, 24,
    17, 61,
    17, 64,
    18, 10,
    18, 62,
    18, 64,
    18, 94,
    19, 21,
    20, 11,
    20, 13,
    20, 15,
    20, 63,
    21, 11,
    21, 13,
    21, 15,
    21, 18,
    21, 24,
    21, 63,
    22, 11,
    22, 13,
    22, 15,
    22, 18,
    22, 63,
    23, 13,
    23, 15,
    23, 64,
    24, 4,
    24, 6,
    24, 7,
    24, 11,
    24, 13,
    24, 15,
    24, 17,
    24, 20,
    24, 21,
    24, 22,
    24, 23,
    24, 24,
    24, 25,
    24, 26,
    24, 27,
    24, 28,
    24, 29,
    24, 63,
    24, 64,
    25, 11,
    25, 13,
    25, 15,
    25, 63,
    26, 13,
    26, 15,
    26, 24,
    26, 61,
    26, 64,
    27, 61,
    28, 61,
    30, 61,
    31, 24,
    31, 61,
    33, 13,
    33, 15,
    33, 16,
    33, 61,
    33, 64,
    60, 93,
    61, 3,
    61, 5,
    61, 8,
    61, 11,
    61, 12,
    61, 14,
    61, 16,
    61, 17,
    61, 18,
    61, 23,
    61, 29,
    61, 30,
    61, 32,
    61, 33,
    61, 61,
    61, 63,
    61, 95,
    63, 7,
    63, 13,
    63, 15,
    63, 21,
    63, 33,
    63, 64,
    64, 11,
    64, 17,
    64, 18,
    64, 20,
    64, 21,
    64, 22,
    64, 23,
    64, 25,
    64, 26,
    64, 33,
    64, 61,
    64, 63,
    64, 93,
    92, 93,
    93, 10,
    93, 62,
    93, 64,
    93, 94,
    95, 19,
    95, 20,
    95, 24,
    95, 61
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -4, -11, -11, -8, 0, -4, -4, -8,
    -4, -11, -11, -8, 2, -4, -17, -17,
    -6, -2, -9, -3, -1, -4, -6, -4,
    -2, -13, -13, -4, -12, 0, -3, -1,
    -4, -3, -3, -2, -13, -6, -3, -1,
    -4, -6, -4, -2, -13, -13, -4, -12,
    0, -3, -1, -4, -3, -3, -2, -13,
    -6, -5, -5, 1, -4, -4, -2, -4,
    -6, 2, 2, 2, 2, -2, -1, -2,
    -2, -1, -5, -4, -4, -5, -1, -5,
    -1, -3, -3, -1, -1, -4, -4, -6,
    -7, 0, -6, 1, -16, -16, -2, -2,
    -7, -1, -2, 3, -2, -1, -4, -4,
    -12, 1, -20, -1, -2, -2, -1, -4,
    -4, -2, -4, -6, -8, -8, -9, -9,
    -9, -6, -6, -5, -5, -5, 2, -10,
    -2, -10, -12, -4, -4, 2, -1, -8,
    -1, -5, -6, -8, -5, -7, -12, -4,
    -4, -17, -17, -6, -2, -9, -9, -6,
    -14, -6, -7, -6, -6, -6, -6, -5,
    -10, -9, 3, 2, 2, 2, 2, 2,
    -3, -1, -4, -6
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 172,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t inter_bold_8px_basicLatin_1b = {
#else
lv_font_t inter_bold_8px_basicLatin_1b = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if INTER_BOLD_8PX_BASICLATIN_1B*/

