#include "menu.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/render.h"

void render_menu_ui(struct LCD *lcd) {
  enum ZH_CH_CHARACTERS title[] = {ZHI,  NENG, SHENG, CHAN,  CHE, JIAN, HUAN,
                                   JING, SHU,  JU,    JIAN2, CE,  XI,   TONG};
  render_zh_cn_string(lcd, title, 14, 30, 40, BLACK, lcd->background_color);

  render_container(lcd, 360, 480, 90, 40, BLACK, SLATE_GRAY);
  render_container(lcd, 300, 205, 30, 555, BLACK, SLATE_GRAY);
  render_container(lcd, 100, 205, 350, 555, BLACK, SLATE_GRAY);

  render_time(lcd, 391, 565, BLACK, SLATE_GRAY);
}

void render_about_us(struct LCD *lcd) {
  render_string(lcd, "By", 340, 420, BLACK, lcd->background_color);
  enum ZH_CH_CHARACTERS name_0[] = {ZENG, QING, JIE};
  render_zh_cn_string(lcd, name_0, 3, 340, 460, BLACK, lcd->background_color);
  render_string(lcd, "20220440333", 340, 560, BLACK, lcd->background_color);

  enum ZH_CH_CHARACTERS name_1[] = {WEN, JIA, JUN};
  render_zh_cn_string(lcd, name_1, 3, 380, 460, BLACK, lcd->background_color);
  render_string(lcd, "20220440333", 380, 560, BLACK, lcd->background_color);
  // TO-DO
}

enum MENU select_menu(struct Touch *touch) {
  enum MOVE type;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  // && touch->y >= 30 && touch->y <= 330
  if (x >= 555 && x <= 760)
    if (y >= 30 && y <= 330) return EXIT;

  return INVALID;
}
