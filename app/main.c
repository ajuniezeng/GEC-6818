#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "bmp.h"
#include "lcd_control.h"

int main(void) {
  struct LCD lcd;
  lcd_new(&lcd);
  lcd.clear(&lcd);
  lcd.draw_dark_full_screen(&lcd);

  render_bmp(&lcd, "pic/pic2.bmp", 100, 200);

  lcd_destructor(&lcd);
  return 0;
}
