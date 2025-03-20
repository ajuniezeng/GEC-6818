#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "font.h"
#include "lcd_control.h"
#include "render.h"

int main(void) {
  struct LCD lcd;
  lcd_new(&lcd);
  lcd.clear(&lcd);
  lcd.draw_background(&lcd, SLATE_GRAY);

  enum ZH_CH_CHARACTERS title[] = {ZHI,  NENG, SHENG, CHAN,  CHE, JIAN, HUAN,
                                   JING, SHU,  JU,    JIAN2, CE,  XI,   TONG};
  render_zh_cn_string(&lcd, title, 14, 20, 100, BLACK, lcd.background_color);

  render_string(&lcd, "By", 340, 420, BLACK, lcd.background_color);
  enum ZH_CH_CHARACTERS name_0[] = {ZENG, QING, JIE2};
  render_zh_cn_string(&lcd, name_0, 3, 340, 460, BLACK, lcd.background_color);
  render_string(&lcd, "20220440333", 340, 560, BLACK, lcd.background_color);

  enum ZH_CH_CHARACTERS name_1[] = {WEN, JIA, JUN};
  render_zh_cn_string(&lcd, name_1, 3, 460, 500, BLACK, lcd.background_color);
  render_string(&lcd, "20220440333", 380, 560, BLACK, lcd.background_color);

  lcd_destructor(&lcd);
  return 0;
}
