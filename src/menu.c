#include "menu.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/lcd_control.h"

enum MENU check_main_menu_option(size_t x, size_t y) {
  // Main menu options
  if (x >= 605 && x <= 760) {
    if (y >= 61 && y <= 119) {
      return LIGHT_CONTROL;
    }

    if (y >= 199 && y <= 257) {
      return SMOKE_DETECTION;
    }

    if (y >= 268 && y <= 326) {
      return EXIT;
    }
  }

  if (x >= 595 && x <= 770) {
    if (y >= 130 && y <= 188) {
      return TEMPERATURE_AND_HUMIDITY_DETECTION;
    }
  }

  return INVALID;
}

enum MENU select_menu_led_control(struct Ui *ui, struct LedStatus *led_status) {
  enum MOVE type;
  struct Touch *touch = &ui->touch;

  if (ui->need_redraw) {
    ui->draw_menu_led_control(ui, led_status);
    ui->need_redraw = 0;
  }

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  enum MENU main_option = check_main_menu_option(x, y);

  if (main_option != INVALID) {
    ui->need_redraw = 1;
    return main_option;
  }

  if (x >= 77 && x <= 326) {
    if (y >= 120 && y <= 190) return SELECT_LED0;
    if (y >= 200 && y <= 270) return SELECT_LED1;
    if (y >= 280 && y <= 345) return SELECT_LED2;
    if (y >= 355 && y <= 415) return SELECT_LED3;
  }
  return INVALID;
}

enum MENU select_menu_temperature_humidity_detection(struct Ui *ui) {
  enum MOVE type;
  struct Touch *touch = &ui->touch;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  enum MENU main_option = check_main_menu_option(x, y);

  if (main_option != INVALID) {
    ui->need_redraw = 1;
    return main_option;
  }

  return INVALID;
}

enum MENU select_menu_smoke_detection(struct Ui *ui) {
  enum MOVE type;
  struct Touch *touch = &ui->touch;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  enum MENU main_option = check_main_menu_option(x, y);

  if (main_option != INVALID) {
    ui->need_redraw = 1;
    return main_option;
  }

  return INVALID;
}

enum MENU prompt_window(struct Ui *ui) {
  struct Touch *touch = &ui->touch;
  size_t prompt_window_width = ui->prompt_window_width;
  enum MOVE type;

  while ((type = touch->get_move(touch)) != TAP) continue;

  size_t x = abs(touch->x);
  size_t y = abs(touch->y);

  printf("(x = %lu, y = %lu)\n", x, y);

  const size_t row = 290;
  const size_t height = 32 + 8;
  size_t yes_column_start = SCREEN_WIDTH / 2 - (prompt_window_width / 2);
  size_t yes_column_end = yes_column_start + prompt_window_width / 2;
  size_t no_column_end = yes_column_end + prompt_window_width / 2;

  if (x >= yes_column_start && x <= yes_column_end && y >= row && y <= row + height) {
    return YES;
  }

  if (x >= yes_column_end && x <= no_column_end && y >= row && y <= row + height) {
    return CANCEL;
  }

  return INVALID;
}
