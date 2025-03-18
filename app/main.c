#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "lcd_control.h"

int main(void) {
  int device = open("/dev/fb0", O_RDWR);
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  for (int i = 0; i < 480; i++) {
    for (int j = 0; j < 800; j++) {
      draw_pixel(device, j, i, BLUE);
    }
  }

  // display a rectangle starting at (100, 100) with width 200 and height 300
  for (int i = 99; i < 400; i++) {
    for (int j = 99; j < 300; j++) {
      draw_pixel(device, j, i, WHITE);
    }
  }
  close(device);
  return 0;
}
