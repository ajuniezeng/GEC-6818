#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

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

int buffer[480][800];

int draw_pixel(int device, unsigned int x, unsigned int y, unsigned int color) {
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  if (x >= 800 || y >= 480) {
    fprintf(stderr, "Invalid coordinates\n");
    return -1;
  }

  buffer[y][x] = color;

  int write_bytes = pwrite(device, &buffer[y][x], sizeof(int), (y * 800 + x) * sizeof(int));

  if (write_bytes == -1) {
    fprintf(stderr, "Failed to write to framebuffer\n");
    return -1;
  }
  return 0;
}

int main(void) {
  int device = open("/dev/fb0", O_RDWR);
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  memset(buffer, BLACK, sizeof(buffer));

  for (int i = 0; i < 480; i++) {
    for (int j = 0; j < 800; j++) {
      draw_pixel(device, j, i, BLUE);
    }
  }

  // display a rectangle starting at (100, 100) with width 200 and height 300
  for (int i = 100; i < 400; i++) {
    for (int j = 100; j < 300; j++) {
      draw_pixel(device, j, i, WHITE);
    }
  }
  close(device);
  return 0;
}
