#pragma once

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

int draw_pixel(int device, unsigned int x, unsigned int y, unsigned int color);
