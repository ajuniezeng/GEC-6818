#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// #include "bmp.h"
#include "bmp.h"
#include "lcd_control.h"
#include "render.h"

int main(void) {
  struct LCD lcd;
  lcd_new(&lcd);
  lcd.clear(&lcd);
  // for (size_t i = 0; i < SCREEN_HEIGHT; i++) {
  //   for (size_t j = 0; j < SCREEN_WIDTH; j++) {
  //     lcd.draw_pixel(&lcd, i, j, TEAL);
  //   }
  // }

  // for (size_t i = 6; i < 20; i++) {
  //   render_zh_cn_character(&lcd, i, 50, 20 + (i - 6) * 32 , BLACK, TEAL);
  // }

  // render_string(&lcd, "By", 380, 620, BLACK, TEAL);

  // for (size_t i = 20; i < 23; i++) {
  //   render_zh_cn_character(&lcd, i, 380, 640 + (i - 20) * 32 , BLACK, TEAL);
  // }

  // render_string(&lcd, "20220440333", 380, 700, BLACK, TEAL);
  
  // for (size_t i = 23; i < 26; i++) {
  //   render_zh_cn_character(&lcd, i, 400, 640 + (i - 23) * 32 , BLACK, TEAL);
  // }

  // render_string(&lcd, "20220440333", 400, 700, BLACK, TEAL);

  render_bmp(&lcd, "pic/pic2.bmp", 0, 0);
  lcd_destructor(&lcd);
  return 0;
}
