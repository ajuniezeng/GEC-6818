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

int draw_pixel(int device, unsigned int x, unsigned int y, unsigned int color) {
  if (device == -1) {
    fprintf(stderr, "Failed to open framebuffer device\n");
    return -1;
  }

  int buffer[480][800];

  if (x >= 800 || y >= 480) {
    fprintf(stderr, "Invalid coordinates\n");
    return -1;
  }

  buffer[y][x] = color;

  int write_bytes = write(device, buffer, sizeof(buffer));

  if (write_bytes == -1) {
    fprintf(stderr, "Failed to write to framebuffer\n");
    return -1;
  }
  return 0;
}

int main(void) {
  int device = open("/dev/fb0", O_RDWR);
  for (int i = 0; i < 480; i++) {
    for (int j = 0; j < 800; j++) {
      draw_pixel(device, j, i, BLUE);
    }
  }
  close(device);
  return 0;
}
