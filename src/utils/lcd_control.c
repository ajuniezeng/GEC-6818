#include "lcd_control.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

static void clear(struct LCD *self) {
  if (self == NULL || self->address == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  memset(self->address, 0x00, SCREEN_BYTES);
}

static void draw_pixel(struct LCD *self, size_t height, size_t width, enum COLOR color) {
  if (self == NULL || self->address == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  if (height >= SCREEN_HEIGHT || width >= SCREEN_WIDTH) {
    fprintf(stderr, "Warning: Draw LCD with invalid index = (%zu, %zu)\n", height, width);
    return;
  }

  self->address[height * SCREEN_WIDTH + width] = color;
}

static void draw_background(struct LCD *self, enum COLOR color) {
  if (self == NULL || self->address == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < SCREEN_HEIGHT; i++) {
    for (size_t j = 0; j < SCREEN_WIDTH; j++) {
      self->address[i * SCREEN_WIDTH + j] = color;
    }
  }

  self->background_color = color;
}

void lcd_new(struct LCD *self) {
  if (self == NULL) {
    fprintf(stderr, "LCD constructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  self->device = open(LCD_DEVICE_PATH, O_RDWR);
  if (self->device == -1) {
    perror("Failed to open framebuffer device");
    exit(EXIT_FAILURE);
  }

  self->address = mmap(NULL, SCREEN_BYTES, PROT_WRITE | PROT_READ, MAP_SHARED, self->device, 0);
  if (self->address == MAP_FAILED) {
    perror("Failed to map framebuffer");
    exit(EXIT_FAILURE);
  }

  self->clear = clear;
  self->draw_pixel = draw_pixel;
  self->draw_background = draw_background;
}

void lcd_destructor(struct LCD *self) {
  if (self == NULL || self->device == -1 || self->address == NULL) {
    fprintf(stderr, "LCD double freed\n");
    exit(EXIT_FAILURE);
  }

  close(self->device);
  munmap(self->address, SCREEN_BYTES);

  self->device = -1;
  self->address = NULL;
}

void draw_pixel_memory(uint32_t *addr, size_t row, size_t column, uint32_t color) {
  if (addr == MAP_FAILED) {
    fprintf(stderr, "Failed to map framebuffer\n");
    exit(EXIT_FAILURE);
  }

  if (row >= SCREEN_HEIGHT || column >= SCREEN_WIDTH) {
    fprintf(stderr, "Invalid coordinates\n");
    return;
  }

  addr[row * SCREEN_WIDTH + column] = color;
}
