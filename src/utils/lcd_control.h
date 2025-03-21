#pragma once

#include <stddef.h>
#include <stdint.h>

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 800
#define SCREEN_BYTES (SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t))
#define LCD_DEVICE_PATH "/dev/fb0"

enum COLOR {
  RED = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  BLACK = 0x000000,
  GRAY = 0x808080,
  BACKGROUND = 0xF4F4F5,
  SLATE_GRAY = 0x708090,
  STONE_300 = 0xD6D3D1,
};

struct LCD {
  int device;
  uint32_t *address;
  uint32_t background_color;
  /// Clear the screen
  void (*clear)(struct LCD *self);

  /// Draw a pixel on the screen
  void (*draw_pixel)(struct LCD *self, size_t row, size_t column, enum COLOR color);

  /// Draw the background with a specific color
  void (*draw_background)(struct LCD *self, enum COLOR color);
};

/// Open the framebuffer device and map the framebuffer to the memory
void lcd_new(struct LCD *self);

/// Unmap the framebuffer and close the framebuffer device
void lcd_destructor(struct LCD *self);

/// Obsolete Function
///
/// Please use the functions in the struct `LCD`
void draw_pixel_memory(uint32_t *addr, size_t row, size_t column, uint32_t color);
