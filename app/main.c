#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "display.h"
#include "lcd_control.h"

int main(void) {
  struct LCD lcd;
  lcd_new(&lcd);
  lcd.clear(&lcd);
  lcd.draw_dark_full_screen(&lcd);

  display_string(&lcd, "Hello World!", 100, 200, WHITE, BLACK);

  lcd_destructor(&lcd);
  return 0;
}
