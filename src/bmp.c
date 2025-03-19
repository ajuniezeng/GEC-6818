#include "bmp.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "lcd_control.h"

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

  // if (header[0] != 'B' || header[1] != 'M') {
  //   fprintf(stderr, "Invalid BMP file\n");
  //   exit(EXIT_FAILURE);
  // }

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
      uint32_t color = (alpha << 24) | (red << 16) | (gree << 8) | blue;

      const size_t row_offset = (height < 0 ? i + row : SCREEN_HEIGHT - i - 1 - row);
      const size_t column_offset = (width > 0 ? j + column : SCREEN_WIDTH - j - 1 - column);

      lcd->draw_pixel(lcd, row_offset, column_offset, color);
    }
    offset += padding;
  }

  close(bmp_file);
  free(bmp_data);
}
