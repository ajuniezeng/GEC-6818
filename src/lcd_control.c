#include <stddef.h>
#include <stdint.h>
#define _POSIX_C_SOURCE 200809L

#include "lcd_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int draw_pixel(int device, size_t height, size_t width, uint32_t color) {
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  if (height >= SCREEN_H || width >= SCREEN_W) {
    fprintf(stderr, "Invalid coordinates\n");
    return -1;
  }

  int buffer[SCREEN_H][SCREEN_W];
  buffer[height][width] = color;

  int write_bytes = pwrite(device, &buffer[width][height], sizeof(int), (height * SCREEN_W + width) * sizeof(int));

  if (write_bytes == -1) {
    fprintf(stderr, "Failed to write to framebuffer\n");
    return -1;
  }

  return 0;
}

void draw_pixel_memory(uint32_t *addr, size_t height, size_t width, uint32_t color) {
  if (addr == MAP_FAILED) {
    fprintf(stderr, "Failed to map framebuffer\n");
    exit(EXIT_FAILURE);
  }

  if (height >= SCREEN_H || width >= SCREEN_W) {
    fprintf(stderr, "Invalid coordinates\n");
    return;
  }

  addr[height * SCREEN_W + width] = color;
}


static void clear(struct LCD *self) {
  if (self == NULL || self->addr == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  memset(self->addr, 0x00, SCREEN_BYTES);
}

static void draw(struct LCD *self, size_t height, size_t width, uint32_t color) {
  if (self == NULL || self->addr == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  if (height >= SCREEN_H || width >= SCREEN_W) {
    fprintf(stderr, "Warning: Draw LCD with index = (%zu, %zu)\n", height, width);
    return;
  }

  self->addr[height * SCREEN_W + width] = color;
}

void LCD_constructor(struct LCD *self) {
  if (self == NULL) {
    fprintf(stderr, "LCD constructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  self->device = open(LCD_DEV_PATH, O_RDWR);
  if (self->device == -1) {
    perror("Failed to open framebuffer device");
    exit(EXIT_FAILURE);
  }

  self->addr = mmap(NULL, SCREEN_BYTES, PROT_WRITE | PROT_READ, MAP_SHARED, self->device, 0);
  if (self->addr == MAP_FAILED) {
    perror("Failed to map framebuffer");
    exit(EXIT_FAILURE);
  }

  self->clear = clear;
  self->draw = draw;
}

void LCD_destructor(struct LCD *self) {
  if (self == NULL || self->device == -1 || self->addr == NULL) {
    fprintf(stderr, "LCD double freed\n");
    exit(EXIT_FAILURE);
  }

  close(self->device);
  munmap(self->addr, SCREEN_BYTES);

  self->device = -1;
  self->addr = NULL;
}
