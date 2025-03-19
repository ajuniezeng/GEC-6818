#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "lcd_control.h"

int main(void) {
  struct LCD lcd;
  LCD_constructor(&lcd);
  lcd.clear(&lcd);

  for (size_t i = 0; i < SCREEN_H; i++) {
    for (size_t j = 0; j < SCREEN_W; j++) {
      lcd.draw(&lcd, i, j, RED);
    }
  }

  int device = open(LCD_DEV_PATH, O_RDWR);
  uint32_t *addr = mmap(NULL, SCREEN_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED, device, 0);

  for (size_t i = 99; i < 400; i++) {
    for (size_t j = 99; j < 300; j++) {
      draw_pixel_memory(addr, i, j, WHITE);
    }
  }

  sleep(30);

  LCD_destructor(&lcd);
  return 0;
}
