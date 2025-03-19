#pragma once

#include <stddef.h>
#include <stdint.h>

#include "lcd_control.h"

#define BMP_WIDTH_OFFSET 0x12
#define BMP_HEIGHT_OFFSET 0x16
#define BMP_DEPTH_OFFSET 0x1C

void render_bmp(struct LCD *lcd, const char *bmp_path, size_t row, size_t column);
