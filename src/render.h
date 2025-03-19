#pragma once

#define BMP_WIDTH_OFFSET 0x12
#define BMP_HEIGHT_OFFSET 0x16
#define BMP_DEPTH_OFFSET 0x1C

#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "lcd_control.h"

/// Render an ASCII character on the LCD screen.
void render_ascii_character(struct LCD *lcd, char ascii, size_t row, size_t column, enum COLOR color,
                            enum COLOR background_color);

/// Render a string on the LCD screen.
void render_string(struct LCD *lcd, const char *string, size_t row, size_t column, enum COLOR color,
                   enum COLOR background_color);

/// Render a Chinese character on the LCD screen.
void render_zh_cn_character(struct LCD *lcd, enum ZH_CH_CHARACTERS character, size_t row, size_t column,
                            enum COLOR color, enum COLOR background_color);

/// Render a Chinese string on the LCD screen.
void render_zh_cn_string(struct LCD *lcd, const enum ZH_CH_CHARACTERS *string, size_t length, size_t row,
                         size_t column, enum COLOR color, enum COLOR background_color);

void render_bmp(struct LCD *lcd, const char *bmp_path, size_t row, size_t column);
