#include "ui.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils/font.h"
#include "utils/module_control.h"
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

static void draw_menu_led_control(struct Ui *self, struct LedStatus *led_status) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  self->current_ui = SELECT_MENU_LED_CONTROL;
  render_bmp(&self->lcd, "pic/led_control.bmp", 0, 0);
  self->draw_time(self, TIME_PANEL_ROW, TIME_PANEL_COLUMN, BLACK, BACKGROUND);
  self->draw_led_status(self, LED0, led_status->led0);
  self->draw_led_status(self, LED1, led_status->led1);
  self->draw_led_status(self, LED2, led_status->led2);
  self->draw_led_status(self, LED3, led_status->led3);
}

static void draw_led_status(struct Ui *self, enum LED led, int value) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  size_t start_row_1 = 130;
  size_t start_row_2 = 207;
  size_t start_row_3 = 284;
  size_t start_row_4 = 361;
  size_t start_column = 280;
  size_t start_row;
  enum ZH_CH_CHARACTERS led_status = KAI;

  if (value == 1) {
    led_status = KAI;
  } else if (value == -1) {
    led_status = GUAN;
    render_zh_cn_character(&self->lcd, led_status, start_row_1, start_column, BLACK, WHITE);
    render_zh_cn_character(&self->lcd, led_status, start_row_2, start_column, BLACK, WHITE);
    render_zh_cn_character(&self->lcd, led_status, start_row_3, start_column, BLACK, WHITE);
    render_zh_cn_character(&self->lcd, led_status, start_row_4, start_column, BLACK, WHITE);
    return;
  } else {
    led_status = GUAN;
  }

  switch (led) {
    case LED0:
      start_row = start_row_1;
      break;
    case LED1:
      start_row = start_row_2;
      break;
    case LED2:
      start_row = start_row_3;
      break;
    case LED3:
      start_row = start_row_4;
      break;
    default:
      fprintf(stderr, "Invalid LED\n");
      return;
  }

  render_zh_cn_character(&self->lcd, led_status, start_row, start_column, BLACK, WHITE);
}

static void *temperature_update_worker(void *arg) {
  struct TemperatureUpdateArgs *args = (struct TemperatureUpdateArgs *)arg;

  while (args->running) {
    pthread_mutex_lock(&temperature_humidity_mutex);

    // Stop when switch to another UI
    if (args->ui->current_ui != SELECT_MENU_TEMPERATURE_HUMIDITY_DETECTION) {
      args->running = 0;
      pthread_mutex_unlock(&temperature_humidity_mutex);
      return NULL;
    }

    int temperature = get_temperature(args->ui->gy_39_device);
    char temperature_string[10];
    snprintf(temperature_string, 10, "%d", temperature);

    render_string(&args->ui->lcd, temperature_string, args->row, args->column, args->color,
                  args->background_color);
    render_zh_cn_character(&args->ui->lcd, CENTIGRADE, args->row,
                           args->column + strlen(temperature_string) * 16, args->color,
                           args->background_color);

    pthread_mutex_unlock(&temperature_humidity_mutex);

    // Update every 1 seconds
    sleep(1);
  }

  return NULL;
}

static void draw_temperature_status(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  // Stop existing thread if it's running
  if (temperature_update_args.running) {
    pthread_mutex_lock(&temperature_humidity_mutex);
    temperature_update_args.running = 0;
    pthread_mutex_unlock(&temperature_humidity_mutex);
    pthread_join(temperature_update_thread, NULL);
  }

  // Immediate first draw
  size_t start_row = 130;
  size_t start_column = 300;

  set_mode_get_environment(self->gy_39_device);
  sleep(1);
  int temperature = get_temperature(self->gy_39_device);
  char temperature_string[10];
  snprintf(temperature_string, 10, "%d", temperature);

  render_string(&self->lcd, temperature_string, start_row, start_column, BLACK, WHITE);
  render_zh_cn_character(&self->lcd, CENTIGRADE, start_row, start_column + strlen(temperature_string) * 16,
                         BLACK, WHITE);

  // Set up thread arguments
  temperature_update_args.ui = self;
  temperature_update_args.row = start_row;
  temperature_update_args.column = start_column;
  temperature_update_args.color = BLACK;
  temperature_update_args.background_color = WHITE;
  temperature_update_args.running = 1;

  // Create and start thread
  if (pthread_create(&temperature_update_thread, NULL, temperature_update_worker, &temperature_update_args) !=
      0) {
    fprintf(stderr, "Failed to create temperature update thread\n");
    return;
  }
}

static void *humidity_update_worker(void *arg) {
  struct HumidityUpdateArgs *humidity_args = (struct HumidityUpdateArgs *)arg;

  while (humidity_args->running) {
    pthread_mutex_lock(&temperature_humidity_mutex);

    // Stop when switch to another UI
    if (humidity_args->ui->current_ui != SELECT_MENU_TEMPERATURE_HUMIDITY_DETECTION) {
      humidity_args->running = 0;
      pthread_mutex_unlock(&temperature_humidity_mutex);
      return NULL;
    }

    int humidity = get_humidity(humidity_args->ui->gy_39_device);
    char humidity_string[10];
    snprintf(humidity_string, 10, "%d%%", humidity);

    render_string(&humidity_args->ui->lcd, humidity_string, humidity_args->row, humidity_args->column,
                  humidity_args->color, humidity_args->background_color);
    pthread_mutex_unlock(&temperature_humidity_mutex);

    // Update every 1 seconds
    sleep(1);
  }

  return NULL;
}

static void draw_humidity_status(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  // Stop existing thread if it's running
  if (humidity_update_args.running) {
    pthread_mutex_lock(&temperature_humidity_mutex);
    humidity_update_args.running = 0;
    pthread_mutex_unlock(&temperature_humidity_mutex);
    pthread_join(humidity_update_thread, NULL);
  }

  // Immediate first draw
  size_t start_row = 208;
  size_t start_column = 300;

  set_mode_get_environment(self->gy_39_device);
  sleep(1);
  int humidity = get_humidity(self->gy_39_device);
  char humidity_string[10];
  snprintf(humidity_string, 10, "%d%%", humidity);

  render_string(&self->lcd, humidity_string, start_row, start_column, BLACK, WHITE);

  // Set up thread arguments
  humidity_update_args.ui = self;
  humidity_update_args.row = start_row;
  humidity_update_args.column = start_column;
  humidity_update_args.color = BLACK;
  humidity_update_args.background_color = WHITE;
  humidity_update_args.running = 1;

  // Create and start thread
  if (pthread_create(&humidity_update_thread, NULL, humidity_update_worker, &humidity_update_args) != 0) {
    humidity_update_args.running = 0;
    fprintf(stderr, "Failed to create humidity update thread\n");
    return;
  }
}

static void draw_menu_temperature_humidity_detection(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  self->current_ui = SELECT_MENU_TEMPERATURE_HUMIDITY_DETECTION;
  render_bmp(&self->lcd, "pic/temperature_humidity_detection.bmp", 0, 0);
  self->draw_time(self, TIME_PANEL_ROW, TIME_PANEL_COLUMN, BLACK, BACKGROUND);
  self->draw_temperature_status(self);
  self->draw_humidity_status(self);
}

static void *smoke_update_worker(void *arg) {
  struct SmokeUpdateArgs *smoke_args = (struct SmokeUpdateArgs *)arg;

  while (smoke_args->running) {
    pthread_mutex_lock(&smoke_mutex);

    // Stop when switch to another UI
    if (smoke_args->ui->current_ui != SELECT_MENU_SMOKE_DETECTION) {
      smoke_args->running = 0;
      pthread_mutex_unlock(&smoke_mutex);
      return NULL;
    }

    int smoke_concentration = get_smoke_concentration(smoke_args->ui->z_mq_01_device);
    char smoke_string[10];
    snprintf(smoke_string, 10, "%d", smoke_concentration);

    render_string(&smoke_args->ui->lcd, smoke_string, smoke_args->row, smoke_args->column,
                  smoke_args->color, smoke_args->background_color);
    pthread_mutex_unlock(&smoke_mutex);

    // Update every 1 seconds
    sleep(1);
  }

  return NULL;
}

static void draw_smoke_status(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  // Stop existing thread if it's running
  if (smoke_update_args.running) {
    pthread_mutex_lock(&smoke_mutex);
    smoke_update_args.running = 0;
    pthread_mutex_unlock(&smoke_mutex);
    pthread_join(smoke_update_thread, NULL);
  }

  // Immediate first draw
  size_t start_row = 130;
  size_t start_column = 305;

  int smoke_concentration = get_smoke_concentration(self->z_mq_01_device);
  char smoke_string[10];
  snprintf(smoke_string, 10, "%d", smoke_concentration);
  render_string(&self->lcd, smoke_string, start_row, start_column, BLACK, WHITE);

  // Set up thread arguments
  smoke_update_args.ui = self;
  smoke_update_args.row = start_row;
  smoke_update_args.column = start_column;
  smoke_update_args.color = BLACK;
  smoke_update_args.background_color = WHITE;
  smoke_update_args.running = 1;

  // Create and start thread
  if (pthread_create(&smoke_update_thread, NULL, smoke_update_worker, &smoke_update_args) != 0) {
    smoke_update_args.running = 0;
    fprintf(stderr, "Failed to create smoke update thread\n");
    return;
  }
}

static void draw_menu_smoke_detection(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  self->current_ui = SELECT_MENU_SMOKE_DETECTION;
  render_bmp(&self->lcd, "pic/smoke_detection.bmp", 0, 0);
  self->draw_time(self, TIME_PANEL_ROW, TIME_PANEL_COLUMN, BLACK, BACKGROUND);
  self->draw_smoke_status(self);
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

  self->previous_ui = self->current_ui;
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
    time_update_args.running = 0;
    fprintf(stderr, "Failed to create time update thread\n");
    return;
  }
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
  self->need_redraw = 1;
  self->z_mq_01_device = uart_init(CON2_PATH);
  self->gy_39_device = uart_init(CON3_PATH);

  self->lcd.clear(&self->lcd);
  self->lcd.draw_background(&self->lcd, WHITE);

  self->draw_window = draw_window;
  self->draw_time = draw_time;
  self->draw_menu_led_control = draw_menu_led_control;
  self->draw_prompt_window = draw_prompt_window;
  self->draw_led_status = draw_led_status;
  self->draw_temperature_status = draw_temperature_status;
  self->draw_humidity_status = draw_humidity_status;
  self->draw_menu_temperature_humidity_detection = draw_menu_temperature_humidity_detection;
  self->draw_menu_smoke_detection = draw_menu_smoke_detection;
  self->draw_smoke_status = draw_smoke_status;
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

  // Stop temperature update thread if running
  if (temperature_update_args.running) {
    pthread_mutex_lock(&temperature_humidity_mutex);
    temperature_update_args.running = 0;
    pthread_mutex_unlock(&temperature_humidity_mutex);
    pthread_join(temperature_update_thread, NULL);
  }

  // Stop humidity update thread if running
  if (humidity_update_args.running) {
    pthread_mutex_lock(&temperature_humidity_mutex);
    humidity_update_args.running = 0;
    pthread_mutex_unlock(&temperature_humidity_mutex);
    pthread_join(humidity_update_thread, NULL);
  }

  // Stop smoke update thread if running
  if (smoke_update_args.running) {
    pthread_mutex_lock(&smoke_mutex);
    smoke_update_args.running = 0;
    pthread_mutex_unlock(&smoke_mutex);
    pthread_join(smoke_update_thread, NULL);
  }

  close(self->z_mq_01_device);
  close(self->gy_39_device);
  touch_destructor(&self->touch);
  lcd_destructor(&self->lcd);
}
