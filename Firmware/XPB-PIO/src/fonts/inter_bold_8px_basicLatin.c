/*******************************************************************************
 * Size: 8 px
 * Bpp: 2
 * Opts: --bpp 2 --size 8 --no-compress --font Inter_18pt-Bold.ttf --range 32-127 --format lvgl -o inter_bold_8px_basicLatin.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef INTER_BOLD_8PX_BASICLATIN
#define INTER_BOLD_8PX_BASICLATIN 1
#endif

#if INTER_BOLD_8PX_BASICLATIN

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x77, 0x77, 0x11, 0x70,

    /* U+0022 "\"" */
    0x3a, 0x39, 0x25,

    /* U+0023 "#" */
    0x15, 0x42, 0x64, 0xbf, 0xc3, 0x30, 0xff, 0x82,
    0x30, 0x66, 0x0,

    /* U+0024 "$" */
    0x4, 0xb, 0xc7, 0xa9, 0xe0, 0x2f, 0x42, 0xbb,
    0xac, 0xbd, 0x4, 0x0,

    /* U+0025 "%" */
    0x3c, 0x20, 0x68, 0x60, 0x68, 0xc0, 0x29, 0x40,
    0x2, 0x2c, 0x8, 0x69, 0x8, 0x3c,

    /* U+0026 "&" */
    0x2d, 0x3, 0x30, 0x3a, 0x3, 0xc4, 0xae, 0xca,
    0x78, 0x3e, 0xc0,

    /* U+0027 "'" */
    0x33, 0x20,

    /* U+0028 "(" */
    0x0, 0x93, 0xc, 0x71, 0xc3, 0xc, 0x28,

    /* U+0029 ")" */
    0x1, 0x83, 0xc, 0x34, 0xd3, 0xc, 0xa0,

    /* U+002A "*" */
    0x18, 0x3d, 0x3e, 0x4,

    /* U+002B "+" */
    0xc, 0x3, 0x7, 0xf8, 0x34, 0x4, 0x0,

    /* U+002C "," */
    0x26, 0x90,

    /* U+002D "-" */
    0x7c, 0x0,

    /* U+002E "." */
    0x17,

    /* U+002F "/" */
    0xc, 0x62, 0x4c, 0x31, 0x89, 0x30,

    /* U+0030 "0" */
    0x2f, 0x7, 0x28, 0xa1, 0xca, 0xc, 0xa1, 0xc7,
    0x28, 0x2f, 0x0,

    /* U+0031 "1" */
    0x3e, 0xb1, 0xc7, 0x1c, 0x71, 0xc0,

    /* U+0032 "2" */
    0x2e, 0x1c, 0xd0, 0x24, 0x1c, 0xd, 0xe, 0xb,
    0xf8,

    /* U+0033 "3" */
    0x2f, 0x1c, 0xd0, 0x24, 0x3c, 0x2, 0xac, 0xa2,
    0xf0,

    /* U+0034 "4" */
    0x7, 0x40, 0xf4, 0x1b, 0x43, 0x34, 0x73, 0x4b,
    0xfc, 0x3, 0x40,

    /* U+0035 "5" */
    0x7f, 0x58, 0x7, 0xe1, 0x8d, 0x2, 0xa8, 0xd2,
    0xe0,

    /* U+0036 "6" */
    0x2f, 0x1c, 0xea, 0x2, 0xbc, 0xb2, 0x9c, 0xa2,
    0xf0,

    /* U+0037 "7" */
    0xbf, 0x40, 0xc0, 0x70, 0x34, 0x1c, 0xa, 0x3,
    0x0,

    /* U+0038 "8" */
    0x2f, 0x1c, 0xa7, 0x24, 0xfc, 0xb2, 0xac, 0xa3,
    0xf0,

    /* U+0039 "9" */
    0x2e, 0x1c, 0xea, 0x2a, 0xcf, 0x3e, 0x98, 0xd3,
    0xe0,

    /* U+003A ":" */
    0x71, 0x1, 0x70,

    /* U+003B ";" */
    0x71, 0x0, 0x26, 0x90,

    /* U+003C "<" */
    0x0, 0x0, 0xa2, 0xe1, 0xd0, 0x1f, 0x40, 0x50,

    /* U+003D "=" */
    0x3f, 0x80, 0x3, 0xf8, 0x0,

    /* U+003E ">" */
    0x0, 0xd, 0x1, 0xf4, 0xe, 0x2e, 0x8, 0x0,

    /* U+003F "?" */
    0x3e, 0x29, 0xc0, 0x70, 0x34, 0x18, 0x1, 0x1,
    0x80,

    /* U+0040 "@" */
    0xb, 0xe0, 0x38, 0x1c, 0x73, 0xf9, 0x99, 0x76,
    0x9c, 0x33, 0x99, 0x76, 0x63, 0xfc, 0x38, 0x0,
    0xb, 0xf0,

    /* U+0041 "A" */
    0xf, 0x1, 0xf0, 0x2b, 0x43, 0x68, 0x3f, 0xc7,
    0xd, 0xa0, 0xa0,

    /* U+0042 "B" */
    0xbf, 0x28, 0xaa, 0x2a, 0xfc, 0xa1, 0xe8, 0xbb,
    0xf4,

    /* U+0043 "C" */
    0x1f, 0x83, 0x5d, 0xa0, 0x4a, 0x0, 0xa0, 0x43,
    0x5d, 0x1f, 0x80,

    /* U+0044 "D" */
    0xbf, 0xb, 0x2c, 0xa0, 0xda, 0xd, 0xa0, 0xdb,
    0x2c, 0xbf, 0x0,

    /* U+0045 "E" */
    0xbf, 0x68, 0xa, 0x2, 0xfc, 0xb0, 0x28, 0xb,
    0xf4,

    /* U+0046 "F" */
    0xbf, 0x68, 0xa, 0x2, 0xfc, 0xb0, 0x28, 0xa,
    0x0,

    /* U+0047 "G" */
    0x1f, 0x83, 0x5d, 0xa0, 0xa, 0x3e, 0xa0, 0xa3,
    0x5d, 0x1f, 0x80,

    /* U+0048 "H" */
    0xa0, 0xda, 0xd, 0xa0, 0xdb, 0xfd, 0xb0, 0xda,
    0xd, 0xa0, 0xd0,

    /* U+0049 "I" */
    0xaa, 0xaa, 0xaa, 0xa0,

    /* U+004A "J" */
    0x3, 0x0, 0xc0, 0x30, 0xc, 0x43, 0x29, 0xc3,
    0xd0,

    /* U+004B "K" */
    0xa1, 0xca, 0x34, 0xaa, 0xb, 0xe0, 0xb7, 0xa,
    0x28, 0xa0, 0xc0,

    /* U+004C "L" */
    0xa0, 0x28, 0xa, 0x2, 0x80, 0xa0, 0x28, 0xb,
    0xf4,

    /* U+004D "M" */
    0xb4, 0x3e, 0xf1, 0xfb, 0xcb, 0xeb, 0x7b, 0xaa,
    0xde, 0x9e, 0x7a, 0x35, 0xc0,

    /* U+004E "N" */
    0xb0, 0xab, 0x8a, 0xbc, 0xaa, 0xaa, 0xa3, 0xea,
    0x2e, 0xa0, 0xe0,

    /* U+004F "O" */
    0x1f, 0x83, 0x5d, 0xa0, 0xaa, 0x7, 0xa0, 0xa3,
    0x5d, 0x1f, 0x80,

    /* U+0050 "P" */
    0xbf, 0x28, 0xaa, 0x1e, 0x8a, 0xbf, 0x2c, 0xa,
    0x0,

    /* U+0051 "Q" */
    0x1f, 0x83, 0x5d, 0xa0, 0xaa, 0x7, 0xa1, 0xa3,
    0x7d, 0x1f, 0xc0, 0x5,

    /* U+0052 "R" */
    0xbf, 0x4a, 0x2c, 0xa1, 0xcb, 0xf4, 0xb7, 0xa,
    0x34, 0xa1, 0xc0,

    /* U+0053 "S" */
    0x2f, 0x1c, 0xa7, 0x0, 0xbd, 0x2, 0xec, 0xb2,
    0xf4,

    /* U+0054 "T" */
    0xbf, 0xc0, 0xd0, 0xd, 0x0, 0xd0, 0xd, 0x0,
    0xd0, 0xd, 0x0,

    /* U+0055 "U" */
    0xa0, 0xda, 0xd, 0xa0, 0xda, 0xd, 0x60, 0xd7,
    0x5c, 0x2f, 0x40,

    /* U+0056 "V" */
    0xa0, 0xa7, 0xd, 0x30, 0xc3, 0x6c, 0x2a, 0x40,
    0xf0, 0xf, 0x0,

    /* U+0057 "W" */
    0xa2, 0xc7, 0x28, 0xf2, 0x87, 0x3c, 0xa0, 0xde,
    0x74, 0x3a, 0x6c, 0xb, 0x4f, 0x1, 0xc3, 0x80,

    /* U+0058 "X" */
    0x70, 0xc3, 0xa8, 0x1f, 0x0, 0xf0, 0x1f, 0x43,
    0x6c, 0xb0, 0xd0,

    /* U+0059 "Y" */
    0xb0, 0xd3, 0x1c, 0x2b, 0x40, 0xf0, 0xa, 0x0,
    0xa0, 0xa, 0x0,

    /* U+005A "Z" */
    0xbf, 0xc0, 0xd0, 0x70, 0x34, 0x28, 0xc, 0xb,
    0xfc,

    /* U+005B "[" */
    0x79, 0xc7, 0x1c, 0x71, 0xc7, 0x1e, 0x0,

    /* U+005C "\\" */
    0xc2, 0x46, 0xc, 0x30, 0x91, 0x83,

    /* U+005D "]" */
    0xb0, 0xd3, 0x4d, 0x34, 0xd3, 0x6d, 0x0,

    /* U+005E "^" */
    0x0, 0x38, 0x68, 0x45,

    /* U+005F "_" */
    0xff, 0x0,

    /* U+0060 "`" */
    0x0, 0xc0, 0x0,

    /* U+0061 "a" */
    0x3e, 0x14, 0xc7, 0xf2, 0x4c, 0x7b, 0x0,

    /* U+0062 "b" */
    0xa0, 0x28, 0xb, 0xf2, 0xce, 0xa2, 0xac, 0xeb,
    0xf0,

    /* U+0063 "c" */
    0x3e, 0x28, 0x8a, 0x2, 0x8c, 0x3e, 0x0,

    /* U+0064 "d" */
    0x2, 0x80, 0xa3, 0xfa, 0xce, 0xa2, 0xac, 0xe3,
    0xf8,

    /* U+0065 "e" */
    0x3e, 0x28, 0xdb, 0xf6, 0x84, 0x3e, 0x0,

    /* U+0066 "f" */
    0x3c, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x30,

    /* U+0067 "g" */
    0x3f, 0xac, 0xea, 0x2a, 0x8e, 0x3f, 0x84, 0xd3,
    0xe0,

    /* U+0068 "h" */
    0xa0, 0x28, 0xb, 0xf2, 0xcd, 0xa2, 0x68, 0x9a,
    0x24,

    /* U+0069 "i" */
    0x60, 0xaa, 0xaa, 0xa0,

    /* U+006A "j" */
    0x18, 0x2, 0x8a, 0x28, 0xa2, 0x8a, 0x34,

    /* U+006B "k" */
    0xa0, 0x28, 0xa, 0x32, 0xb4, 0xbc, 0x2a, 0x8a,
    0x34,

    /* U+006C "l" */
    0xaa, 0xaa, 0xaa, 0xa0,

    /* U+006D "m" */
    0xbe, 0xb6, 0xdd, 0xaa, 0x31, 0xe8, 0xc7, 0xa3,
    0x1c,

    /* U+006E "n" */
    0xaf, 0x2d, 0xda, 0x26, 0x89, 0xa2, 0x40,

    /* U+006F "o" */
    0x3e, 0x2c, 0xda, 0x2a, 0xcd, 0x3e, 0x0,

    /* U+0070 "p" */
    0xbf, 0x2c, 0xaa, 0x2a, 0xce, 0xbf, 0x28, 0xa,
    0x0,

    /* U+0071 "q" */
    0x3f, 0xac, 0xea, 0x2a, 0xce, 0x3f, 0x80, 0xa0,
    0x28,

    /* U+0072 "r" */
    0x0, 0xbc, 0xb0, 0xa0, 0xa0, 0xa0,

    /* U+0073 "s" */
    0x3e, 0x28, 0x43, 0xe1, 0x4c, 0x3e, 0x0,

    /* U+0074 "t" */
    0x1, 0xcf, 0x9c, 0x71, 0xc3, 0x80,

    /* U+0075 "u" */
    0xa2, 0x68, 0x9a, 0x25, 0xcd, 0x3f, 0x40,

    /* U+0076 "v" */
    0x93, 0x5c, 0xc3, 0x70, 0xf4, 0x1c, 0x0,

    /* U+0077 "w" */
    0xd7, 0x2a, 0x9c, 0xd7, 0xab, 0xf, 0x38, 0x28,
    0xd0,

    /* U+0078 "x" */
    0x63, 0xf, 0x82, 0xc0, 0xe8, 0xa3, 0x0,

    /* U+0079 "y" */
    0x92, 0x5c, 0xc3, 0x70, 0xf4, 0x1c, 0x7, 0x7,
    0x40,

    /* U+007A "z" */
    0xbf, 0x3, 0x81, 0xc0, 0xd0, 0xbf, 0x0,

    /* U+007B "{" */
    0xd, 0x28, 0x24, 0x24, 0xb0, 0x34, 0x28, 0x1d,
    0x0,

    /* U+007C "|" */
    0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x0,

    /* U+007D "}" */
    0xb0, 0x30, 0x34, 0x24, 0x1d, 0x38, 0x34, 0xb0,
    0x0,

    /* U+007E "~" */
    0x38, 0x9a, 0xd0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 30, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 41, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 68, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 7, .adv_w = 82, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 84, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 30, .adv_w = 128, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 85, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 41, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 57, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 71, .adv_w = 71, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 75, .adv_w = 86, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 40, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 84, .adv_w = 59, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 86, .adv_w = 40, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 87, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 93, .adv_w = 86, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 54, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 86, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 79, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 74, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 184, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 41, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 191, .adv_w = 86, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 86, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 86, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 72, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 221, .adv_w = 130, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 239, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 94, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 92, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 78, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 75, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 35, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 74, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 72, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 118, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 97, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 83, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 99, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 410, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 85, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 93, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 94, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 132, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 93, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 92, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 517, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 523, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 530, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 534, .adv_w = 61, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 536, .adv_w = 45, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 539, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 546, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 555, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 562, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 578, .adv_w = 50, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 594, .adv_w = 79, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 34, .box_w = 3, .box_h = 9, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 614, .adv_w = 73, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 623, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 627, .adv_w = 116, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 643, .adv_w = 78, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 650, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 659, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 668, .adv_w = 51, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 674, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 681, .adv_w = 47, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 687, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 701, .adv_w = 108, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 73, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 717, .adv_w = 76, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 726, .adv_w = 72, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 733, .adv_w = 59, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 742, .adv_w = 47, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 751, .adv_w = 59, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 760, .adv_w = 86, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 1}
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
    .bpp = 2,
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
const lv_font_t inter_bold_8px_basicLatin = {
#else
lv_font_t inter_bold_8px_basicLatin = {
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



#endif /*#if INTER_BOLD_8PX_BASICLATIN*/

