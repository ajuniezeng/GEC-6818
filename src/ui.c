#include "ui.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils/font.h"
#include "utils/lcd_control.h"
#include "utils/led_control.h"
#include "utils/render.h"

static void draw_window(struct Ui *self, size_t height, size_t width, size_t row, size_t column,
                        enum COLOR color, enum COLOR background_color) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  render_line(&self->lcd, 1, width, row, column, color);
  render_vertical_line(&self->lcd, height, 1, row, column, color);
  render_line(&self->lcd, 1, width, row + height, column, color);
  render_vertical_line(&self->lcd, height, 1, row, column + width - 1, color);

  for (size_t i = 0; i < height - 2; i++) {
    for (size_t j = 0; j < width - 2; j++) {
      self->lcd.draw_pixel(&self->lcd, row + i + 1, column + j + 1, background_color);
    }
  }
}

static void draw_menu_led_control(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  self->current_ui = SELECT_MENU_LED_CONTROL;
  render_bmp(&self->lcd, "pic/led_control.bmp", 0, 0);
  self->draw_time(self, 386, 587, BLACK, BACKGROUND);
}

static void draw_prompt_window(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }
  // Pause Time Update Thread
  pthread_mutex_lock(&time_mutex);
  time_update_args.running = 0;
  pthread_mutex_unlock(&time_mutex);
  pthread_join(time_update_thread, NULL);

  self->current_ui = PROMPT_WINDOW;

  // TO-DO: wrap the prompt text
  size_t prompt_length = length * 32;
  const size_t container_start_row = SCREEN_HEIGHT / 2 - 75;
  size_t container_start_column = SCREEN_WIDTH / 2 - (prompt_length + 10 + 25) / 2;

  const size_t container_height = 180;
  size_t container_width = prompt_length + 10 + 25;

  const size_t prompt_start_row = container_start_row + 20;
  const size_t prompt_start_column = container_start_column + 10;

  const size_t yes_start_row = container_start_row + 130;
  size_t yes_start_column = container_start_column + container_width / 4 - 16;

  const size_t no_start_row = container_start_row + 130;
  size_t no_start_column = container_start_column + container_width / 2 + container_width / 4 - 16;

  self->prompt_window_width = container_width;

  self->draw_window(self, container_height, container_width, container_start_row, container_start_column,
                    BLACK, STONE_300);
  render_zh_cn_string(&self->lcd, prompt, length, prompt_start_row, prompt_start_column, BLACK, STONE_300);
  render_zh_cn_character(&self->lcd, JA, yes_start_row, yes_start_column, RED, STONE_300);
  render_zh_cn_character(&self->lcd, NEIN, no_start_row, no_start_column, BLACK, STONE_300);
}

static void *time_update_worker(void *arg) {
  struct TimeUpdateArgs *args = (struct TimeUpdateArgs *)arg;

  while (args->running) {
    pthread_mutex_lock(&time_mutex);

    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    char time_string[12];
    strftime(time_string, 12, "%H:%M:%S", time_info);

    render_string(&args->ui->lcd, time_string, args->row, args->column, args->color, args->background_color);

    pthread_mutex_unlock(&time_mutex);

    // Update every second
    sleep(1);
  }

  return NULL;
}

static void draw_time(struct Ui *self, size_t row, size_t column, enum COLOR color,
                      enum COLOR background_color) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  // Stop existing thread if it's running
  if (time_update_args.running) {
    pthread_mutex_lock(&time_mutex);
    time_update_args.running = 0;
    pthread_mutex_unlock(&time_mutex);
    pthread_join(time_update_thread, NULL);
  }

  // Immediate first draw
  time_t current_time = time(NULL);
  struct tm *time_info = localtime(&current_time);

  char time_string[12];
  strftime(time_string, 12, "%H/%M/%S", time_info);

  render_string(&self->lcd, time_string, row, column, color, background_color);

  // Set up thread arguments
  time_update_args.ui = self;
  time_update_args.row = row;
  time_update_args.column = column;
  time_update_args.color = color;
  time_update_args.background_color = background_color;
  time_update_args.running = 1;

  // Create and start thread
  if (pthread_create(&time_update_thread, NULL, time_update_worker, &time_update_args) != 0) {
    fprintf(stderr, "Failed to create time update thread\n");
    return;
  }
}

static void draw_led_status(struct Ui *self, enum LED led, int value) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  size_t start_column = 280;
  size_t start_row;
  enum ZH_CH_CHARACTERS led_status = KAI;

  if (value == 1) {
    led_status = KAI;
  } else {
    led_status = GUAN;
  }

  switch (led) {
    case LED0:
      start_row = 130;
      break;
    case LED1:
      start_row = 207;
      break;
    case LED2:
      start_row = 284;
      break;
    case LED3:
      start_row = 361;
      break;
    default:
      fprintf(stderr, "Invalid LED\n");
      return;
  }

  render_zh_cn_character(&self->lcd, led_status, start_row, start_column, BLACK, WHITE);
}

void ui_new(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI constructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  lcd_new(&self->lcd);
  touch_new(&self->touch);
  self->prompt_window_width = 0;
  self->current_ui = SELECT_MENU_LED_CONTROL;
  self->previous_ui = SELECT_MENU_LED_CONTROL;

  self->lcd.clear(&self->lcd);
  self->lcd.draw_background(&self->lcd, WHITE);

  self->draw_window = draw_window;
  self->draw_time = draw_time;
  self->draw_menu_led_control = draw_menu_led_control;
  self->draw_prompt_window = draw_prompt_window;
  self->draw_led_status = draw_led_status;
}

void ui_destructor(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI destructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  // Stop time update thread if running
  if (time_update_args.running) {
    pthread_mutex_lock(&time_mutex);
    time_update_args.running = 0;
    pthread_mutex_unlock(&time_mutex);
    pthread_join(time_update_thread, NULL);
  }

  touch_destructor(&self->touch);
  lcd_destructor(&self->lcd);
}
