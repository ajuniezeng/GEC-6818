#pragma once

#include <stddef.h>
#include <stdint.h>

#define SCREEN_H 480
#define SCREEN_W 800
#define SCREEN_BYTES (SCREEN_H * SCREEN_W * sizeof(uint32_t))
#define LCD_DEV_PATH "/dev/fb0"

enum COLOR {
  RED = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  BLACK = 0x000000
};

struct LCD {
  int device;
  uint32_t *addr;

  void (*clear)(struct LCD *self);
  void (*draw)(struct LCD *self, size_t height, size_t width, uint32_t color);
};

void LCD_constructor(struct LCD *self);
void LCD_destructor(struct LCD *self);

int draw_pixel(int device, size_t x, size_t y, uint32_t color);
void draw_pixel_memory(uint32_t *addr, size_t height, size_t width, uint32_t color);
