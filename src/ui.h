#pragma once

#include <stddef.h>
#include "utils/font.h"
#include "utils/lcd_control.h"
#include "utils/touch.h"

enum UiType {
  SELECT_MENU_NONE,
  SELECT_MENU_SMOKE_DETECTION,
  SELECT_MENU_MOISTURE_DETECTION,
  SELECT_MENU_LED_CONTROL,
  SELECT_MENU_ABOUT_US,
  PROMPT_WINDOW,
};

struct Ui {
  struct LCD lcd;
  struct Touch touch;
  size_t prompt_window_width;
  enum UiType current_ui;

  void (*draw_window)(struct Ui *self, size_t height, size_t width, size_t row, size_t column, enum COLOR color,
                      enum COLOR background_color);
  void (*draw_time)(struct Ui *self, size_t row, size_t column, enum COLOR color, enum COLOR background_color);

  void (*draw_menu)(struct Ui *self);

  void (*draw_about_us)(struct Ui *self);

  void (*draw_prompt_window)(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length);
};

void ui_new(struct Ui *self);

void ui_destructor(struct Ui *self);
