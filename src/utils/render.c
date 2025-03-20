#include "render.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <time.h>
#include <unistd.h>

void render_ascii_character(struct LCD *lcd, char ascii, size_t row, size_t column, enum COLOR color,
                            enum COLOR background_color) {
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
        lcd->draw_pixel(lcd, row + i, column + j, background_color);
      }
    }
  }
}

void render_string(struct LCD *lcd, const char *string, size_t row, size_t column, enum COLOR color,
                   enum COLOR background_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; string[i] != '\0'; i++) {
    render_ascii_character(lcd, string[i], row, column + i * 16, color, background_color);
  }
}

void render_zh_cn_character(struct LCD *lcd, enum ZH_CH_CHARACTERS character, size_t row, size_t column,
                            enum COLOR color, enum COLOR background_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      int byte_index = i * 4 + (j / 8);
      int bit_shift = j % 8;
      if (PING_FANG_SC_32x32[character][byte_index] & (1 << (7 - bit_shift))) {
        lcd->draw_pixel(lcd, row + i, column + j, color);
      } else {
        lcd->draw_pixel(lcd, row + i, column + j, background_color);
      }
    }
  }
}

void render_zh_cn_string(struct LCD *lcd, const enum ZH_CH_CHARACTERS *string, size_t length, size_t row,
                         size_t column, enum COLOR color, enum COLOR background_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < length; i++) {
    render_zh_cn_character(lcd, string[i], row, column + i * 32, color, background_color);
  }
}

void render_bmp(struct LCD *lcd, const char *bmp_path, size_t row, size_t column) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  int bmp_file = open(bmp_path, O_RDONLY);
  if (bmp_file == -1) {
    perror("Failed to open BMP file");
    exit(EXIT_FAILURE);
  }

  // check whether the BMP file is valid
  char header[2];
  read(bmp_file, header, 2);

  if (header[0] != 'B' || header[1] != 'M') {
    fprintf(stderr, "Invalid BMP file\n");
    exit(EXIT_FAILURE);
  }

  int width, height;
  short depth;

  lseek(bmp_file, BMP_WIDTH_OFFSET, SEEK_SET);
  read(bmp_file, &width, 4);

  lseek(bmp_file, BMP_HEIGHT_OFFSET, SEEK_SET);
  read(bmp_file, &height, 4);

  lseek(bmp_file, BMP_DEPTH_OFFSET, SEEK_SET);
  read(bmp_file, &depth, 2);

  if (depth != 24 && depth != 32) {
    fprintf(stderr, "Unsupported BMP depth = %d\n", depth);
    exit(EXIT_FAILURE);
  }

  uint32_t height_abs = abs(height);
  uint32_t width_abs = abs(width);

  size_t row_size = width_abs * depth / 8;
  size_t padding = 0;

  if (row_size % 4 != 0) {
    padding = 4 - row_size % 4;
  }

  size_t bmp_size = (row_size + padding) * height_abs;
  uint8_t *bmp_data = malloc(bmp_size);

  if (bmp_data == NULL) {
    perror("Failed to allocate memory for BMP data");
    exit(EXIT_FAILURE);
  }

  lseek(bmp_file, 54, SEEK_SET);
  read(bmp_file, bmp_data, bmp_size);

  int offset = 0;

  for (size_t i = 0; i < height_abs; i++) {
    for (size_t j = 0; j < width_abs; j++) {
      uint8_t blue = bmp_data[offset++];
      uint8_t gree = bmp_data[offset++];
      uint8_t red = bmp_data[offset++];
      uint8_t alpha = (depth == 32 ? bmp_data[offset++] : 0);
      enum COLOR color = (alpha << 24) | (red << 16) | (gree << 8) | blue;

      const size_t row_offset = (height < 0 ? i + row : SCREEN_HEIGHT - i - 1 - row);
      const size_t column_offset = (width > 0 ? j + column : SCREEN_WIDTH - j - 1 - column);

      lcd->draw_pixel(lcd, row_offset, column_offset, color);
    }
    offset += padding;
  }

  close(bmp_file);
  free(bmp_data);
}

void render_line(struct LCD *lcd, size_t height, size_t width, size_t row, size_t column, enum COLOR color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      lcd->draw_pixel(lcd, row + i, column + j, color);
    }
  }
}

void render_vertical_line(struct LCD *lcd, size_t height, size_t width, size_t row, size_t column,
                          enum COLOR color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      lcd->draw_pixel(lcd, row + i, column, color);
    }
  }
}

void render_container(struct LCD *lcd, size_t height, size_t width, size_t row, size_t column,
                      enum COLOR color, enum COLOR background_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  render_line(lcd, 1, width, row, column, color);
  render_vertical_line(lcd, height, 1, row, column, color);
  render_line(lcd, 1, width, row + height, column, color);
  render_vertical_line(lcd, height, 1, row, column + width - 1, color);

  for (size_t i = 0; i < height - 2; i++) {
    for (size_t j = 0; j < width - 2; j++) {
      lcd->draw_pixel(lcd, row + i + 1, column + j + 1, background_color);
    }
  }
}

void render_time(struct LCD *lcd, size_t row, size_t column, enum COLOR color, enum COLOR background_color) {
  if (lcd == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  time_t current_time = time(NULL);
  struct tm *time_info = localtime(&current_time);

  char time_string[9];
  strftime(time_string, 9, "%Y/%m/%d", time_info);

  render_string(lcd, time_string, row, column, color, background_color);
}
