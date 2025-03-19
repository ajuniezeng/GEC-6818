#include "display.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "font.h"

void display_character(struct LCD *lcd, char ascii, size_t row, size_t column, uint32_t color, uint32_t back_color) {
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

void display_string(struct LCD *lcd, const char *string, size_t row, size_t column, uint32_t color, uint32_t back_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; string[i] != '\0'; i++) {
    display_character(lcd, string[i], row, column + i * 16, color, back_color);
  }
}
