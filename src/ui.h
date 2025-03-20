#pragma once

#include "utils/font.h"
#include "utils/lcd_control.h"
#include "utils/touch.h"

struct Ui {
  struct LCD lcd;
  struct Touch touch;
  size_t prompt_window_width;

  void (*draw_window)(struct Ui *self, size_t height, size_t width, size_t row, size_t column, enum COLOR color,
                      enum COLOR background_color);
  void (*draw_time)(struct Ui *self, size_t row, size_t column, enum COLOR color, enum COLOR background_color);

  void (*draw_menu)(struct Ui *self);

  void (*draw_about_us)(struct Ui *self);

  void (*draw_prompt_window)(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length);
};

void ui_new(struct Ui *self);

void ui_destructor(struct Ui *self);
