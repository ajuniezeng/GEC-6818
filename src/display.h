#pragma once

#include <stddef.h>
#include <stdint.h>
#include "lcd_control.h"

void display_character(struct LCD *lcd, char ascii, size_t row, size_t column, uint32_t color);
void display_string(struct LCD *lcd, const char *string, size_t row, size_t column, uint32_t color);
