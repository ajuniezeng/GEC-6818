#include "lcd_control.h"

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <stdio.h>
#include <sys/fcntl.h>

int draw_pixel(int device, unsigned int x, unsigned int y, unsigned int color) {
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  if (x >= 800 || y >= 480) {
    fprintf(stderr, "Invalid coordinates\n");
    return -1;
  }

  int buffer[480][800];
  buffer[y][x] = color;

  int write_bytes = pwrite(device, &buffer[y][x], sizeof(int), (y * 800 + x) * sizeof(int));

  if (write_bytes == -1) {
    fprintf(stderr, "Failed to write to framebuffer\n");
    return -1;
  }
  return 0;
}
