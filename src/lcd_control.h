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
  PURPLE = 0x800080,
  ORANGE = 0xFFA500,
  PINK = 0xFFC0CB,
  BROWN = 0xA52A2A,
  GRAY = 0x808080,
  SLATE_GRAY = 0x708090,
  AMBER = 0xFFBF00,
  LIME = 0xBFFF00,
  TEAL = 0x008080,
  INDIGO = 0x4B0082,
  MAROON = 0x800000,
  OLIVE = 0x808000,
  NAVY = 0x000080,
  SILVER = 0xC0C0C0,
  GOLD = 0xFFD700,
  SKY_BLUE = 0x87CEEB,
  VIOLET = 0xEE82EE,
  TURQUOISE = 0x40E0D0,
  BEIGE = 0xF5F5DC,
  TAN = 0xD2B48C,
  KHAKI = 0xF0E68C,
  CORAL = 0xFF7F50,
  SALMON = 0xFA8072,
  TOMATO = 0xFF6347,
  ORCHID = 0xDA70D6,
  THISTLE = 0xD8BFD8,
  LAVENDER = 0xE6E6FA,
};

struct LCD {
  int device;
  uint32_t *addr;
  uint32_t background_color;

  /// Clear the screen
  void (*clear)(struct LCD *self);

  /// Draw a pixel on the screen
  void (*draw_pixel)(struct LCD *self, size_t row, size_t column, enum COLOR color);

  /// Make the full screen dark
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
