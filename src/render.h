#pragma once

#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "lcd_control.h"

/// Render an ASCII character on the LCD screen.
void render_ascii_character(struct LCD *lcd, char ascii, size_t row, size_t column, uint32_t color,
                            uint32_t back_color);

/// Render a string on the LCD screen.
void render_string(struct LCD *lcd, const char *string, size_t row, size_t column, uint32_t color,
                   uint32_t back_color);

/// Render a Chinese character on the LCD screen.
void render_zh_cn_character(struct LCD *lcd, enum ZH_CH_CHARACTERS character, size_t row, size_t column,
                            uint32_t color, uint32_t back_color);

/// Render a Chinese string on the LCD screen.
void render_zh_cn_string(struct LCD *lcd, const enum ZH_CH_CHARACTERS *string, size_t length, size_t row,
                         size_t column, uint32_t color, uint32_t back_color);
