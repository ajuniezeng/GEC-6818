#pragma once

#include <stddef.h>
#include <pthread.h>

#include "utils/font.h"
#include "utils/lcd_control.h"
#include "utils/touch.h"
#include "utils/led_control.h"

enum UiType {
  SELECT_MENU_SMOKE_DETECTION,
  SELECT_MENU_MOISTURE_DETECTION,
  SELECT_MENU_LED_CONTROL,
  PROMPT_WINDOW,
};

struct Ui {
  struct LCD lcd;
  struct Touch touch;
  size_t prompt_window_width;
  enum UiType current_ui;
  enum UiType previous_ui;

  void (*draw_window)(struct Ui *self, size_t height, size_t width, size_t row, size_t column,
                      enum COLOR color, enum COLOR background_color);
  void (*draw_time)(struct Ui *self, size_t row, size_t column, enum COLOR color,
                    enum COLOR background_color);

  void (*draw_menu_led_control)(struct Ui *self);

  void (*draw_prompt_window)(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length);

  void (*draw_led_status)(struct Ui *self, enum LED led, int value);
};

void ui_new(struct Ui *self);
void ui_destructor(struct Ui *self);

struct TimeUpdateArgs {
  struct Ui *ui;
  size_t row;
  size_t column;
  enum COLOR color;
  enum COLOR background_color;
  int running;  // Flag to control thread execution
};

static struct TimeUpdateArgs time_update_args = {0};
static pthread_t time_update_thread;
static pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;
