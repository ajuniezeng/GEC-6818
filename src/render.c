#include "render.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "font.h"

void render_ascii_character(struct LCD *lcd, char ascii, size_t row, size_t column, uint32_t color,
                            uint32_t back_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  if (ascii < 0x20 || ascii > 0x7E) {
    fprintf(stderr, "Warning: Display invalid character = %c\n", ascii);
    return;
  }

  int index = ascii - 32;

  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 16; j++) {
      if (ASCII_TABLE[index][i] >> j & 1) {
        lcd->draw_pixel(lcd, row + i, column + j, color);
      } else {
        lcd->draw_pixel(lcd, row + i, column + j, back_color);
      }
    }
  }
}

void render_string(struct LCD *lcd, const char *string, size_t row, size_t column, uint32_t color,
                   uint32_t back_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; string[i] != '\0'; i++) {
    render_ascii_character(lcd, string[i], row, column + i * 16, color, back_color);
  }
}

void render_zh_cn_character(struct LCD *lcd, enum ZH_CH_CHARACTERS character, size_t row, size_t column,
                            uint32_t color, uint32_t back_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      int byte_index = i * 4 + (i / 8);
      int bit_shift = j % 8;
      if (PING_FANG_SC_32x32[character][byte_index] & (1 << (7 - bit_shift))) {
        lcd->draw_pixel(lcd, row + i, column + j, color);
      } else {
        lcd->draw_pixel(lcd, row + i, column + j, back_color);
      }
    }
  }
}

void render_zh_cn_string(struct LCD *lcd, const enum ZH_CH_CHARACTERS *string, size_t length, size_t row,
                         size_t column, uint32_t color, uint32_t back_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < length; i++) {
    render_zh_cn_character(lcd, string[i], row, column + i * 8, color, back_color);
  }
}
