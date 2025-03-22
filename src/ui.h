#pragma once

#include <pthread.h>
#include <stddef.h>

#include "utils/font.h"
#include "utils/lcd_control.h"
#include "utils/led_control.h"
#include "utils/touch.h"

#define TIME_PANEL_ROW 386
#define TIME_PANEL_COLUMN 600

enum UiType {
  SELECT_MENU_SMOKE_DETECTION,
  SELECT_MENU_TEMPERATURE_HUMIDITY_DETECTION,
  SELECT_MENU_LED_CONTROL,
  PROMPT_WINDOW,
};

struct Ui {
  struct LCD lcd;
  struct Touch touch;
  size_t prompt_window_width;
  enum UiType current_ui;
  enum UiType previous_ui;
  int need_redraw;
  int gy_39_device;
  int z_mq_01_device;

  void (*draw_window)(struct Ui *self, size_t height, size_t width, size_t row, size_t column,
                      enum COLOR color, enum COLOR background_color);
  void (*draw_time)(struct Ui *self, size_t row, size_t column, enum COLOR color,
                    enum COLOR background_color);

  void (*draw_menu_led_control)(struct Ui *self, struct LedStatus *led_status);
  void (*draw_menu_temperature_humidity_detection)(struct Ui *self);
  void (*draw_menu_smoke_detection)(struct Ui *self);

  void (*draw_prompt_window)(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length);

  void (*draw_led_status)(struct Ui *self, enum LED led, int value);
  void (*draw_temperature_status)(struct Ui *self);
  void (*draw_humidity_status)(struct Ui *self);
  void (*draw_smoke_status)(struct Ui *self);
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

struct TemperatureUpdateArgs {
  struct Ui *ui;
  size_t row;
  size_t column;
  enum COLOR color;
  enum COLOR background_color;
  int running;  // Flag to control thread execution
  int value; // Temperature value to display
};

struct HumidityUpdateArgs {
  struct Ui *ui;
  size_t row;
  size_t column;
  enum COLOR color;
  enum COLOR background_color;
  int running;  // Flag to control thread execution
  int value; // Humidity value to display
};

static struct TemperatureUpdateArgs temperature_update_args = {0};
static struct HumidityUpdateArgs humidity_update_args = {0};
static pthread_t temperature_update_thread;
static pthread_t humidity_update_thread;
static pthread_mutex_t temperature_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t humidity_mutex = PTHREAD_MUTEX_INITIALIZER;

struct SmokeUpdateArgs {
  struct Ui *ui;
  size_t row;
  size_t column;
  enum COLOR color;
  enum COLOR background_color;
  int running;  // Flag to control thread execution
  int value; // Smoke concentration value to display
};

static struct SmokeUpdateArgs smoke_update_args = {0};
static pthread_t smoke_update_thread;
static pthread_mutex_t smoke_mutex = PTHREAD_MUTEX_INITIALIZER;
