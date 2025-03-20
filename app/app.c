#include "app.h"
#include <stdio.h>

#include "menu.h"
#include "utils/lcd_control.h"
#include "utils/touch.h"

void start(void) {
  struct LCD lcd;
  lcd_new(&lcd);
  lcd.clear(&lcd);
  lcd.draw_background(&lcd, WHITE);

  struct Touch touch;
  touch_new(&touch);

  render_menu_ui(&lcd);

  while (1) {
    enum MENU menu;

    while ((menu = select_menu(&touch) == INVALID)) continue;

    printf("%d\n", menu);
    if (menu == EXIT) break;
  }

  touch_destructor(&touch);
  lcd_destructor(&lcd);
}
