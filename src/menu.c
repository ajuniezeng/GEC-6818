#include "menu.h"

#include <stdio.h>
#include <stdlib.h>

enum MENU select_menu(struct Touch *touch) {
  enum MOVE type;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  // && touch->y >= 30 && touch->y <= 330
  if (x >= 550 && x <= 760) {
    if (y >= 260 && y <= 318) {
      return EXIT;
    }
  }

  if (x >= 550 && x <= 760) {
    if (y >= 50 && y <= 88) {
      return TURN_ON_SLASH_OFF_LED1;
    }
  }

  if (x >= 550 && x <= 760) {
    if (y >= 98 && y <= 136) {
      return TURN_ON_SLASH_OFF_LED2;
    }
  }

  if (x >= 550 && x <= 760) {
    if (y >= 146 && y <= 184) {
      return TURN_ON_SLASH_OFF_LED3;
    }
  }

  if (x >= 550 && x <= 760) {
    if (y >= 194 && y <= 232) {
      return TRUN_ON_SLASH_OFF_BEEP;
    }
  }

  return INVALID;
}

enum MENU prompt_window(struct Touch *touch) {
  enum MOVE type;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  if (x >= 200 && x <= 600) {
    if (y >= 200 && y <= 280) {
      return EXIT;
    }
  }

  return INVALID;
}
