#include "ui.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils/font.h"
#include "utils/lcd_control.h"
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

static void draw_menu(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  enum ZH_CH_CHARACTERS title[] = {ZHI,  NENG, SHENG, CHAN,  CHE, JIAN, HUAN,
                                   JING, SHU,  JU,    JIAN2, CE,  XI,   TONG};
  render_zh_cn_string(&self->lcd, title, 14, 30, 40, BLACK, self->lcd.background_color);

  self->draw_window(self, 360, 480, 90, 40, BLACK, SLATE_GRAY);
  self->draw_window(self, 300, 205, 30, 555, BLACK, SLATE_GRAY);
  self->draw_window(self, 100, 205, 350, 555, BLACK, SLATE_GRAY);

  self->draw_time(self, 391, 565, BLACK, SLATE_GRAY);
  const enum ZH_CH_CHARACTERS command_CN[] = {KAI, SLASH, GUAN};
  const enum ZH_CH_CHARACTERS command_exit_CN[] = {TUI, CHU};

  const size_t command_1_start_row = 50;
  const size_t command_1_start_column = 587;
  const char command_1_ASICC[] = "LED1";

  const size_t command_2_start_row = 98;
  const size_t command_2_start_column = 587;
  const char command_2_ASICC[] = "LED2";

  const size_t command_3_start_row = 146;
  const size_t command_3_start_column = 587;
  const char command_3_ASICC[] = "LED3";

  const size_t command_4_start_row = 194;
  const size_t command_4_start_column = 587;
  const char command_4_ASICC[] = "BEEP";

  const size_t exit_command_start_row = 625;
  const size_t exit_command_start_column = 270;

  render_zh_cn_string(&self->lcd, command_CN, 3, command_1_start_row, command_1_start_column, BLACK,
                      SLATE_GRAY);
  render_string(&self->lcd, command_1_ASICC, command_1_start_row, command_1_start_column + 32 * 3, BLACK,
                SLATE_GRAY);

  render_zh_cn_string(&self->lcd, command_CN, 3, command_2_start_row, command_2_start_column, BLACK,
                      SLATE_GRAY);
  render_string(&self->lcd, command_2_ASICC, command_2_start_row, command_2_start_column + 32 * 3, BLACK,
                SLATE_GRAY);

  render_zh_cn_string(&self->lcd, command_CN, 3, command_3_start_row, command_3_start_column, BLACK,
                      SLATE_GRAY);
  render_string(&self->lcd, command_3_ASICC, command_3_start_row, command_3_start_column + 32 * 3, BLACK,
                SLATE_GRAY);

  render_zh_cn_string(&self->lcd, command_CN, 3, command_4_start_row, command_4_start_column, BLACK,
                      SLATE_GRAY);
  render_string(&self->lcd, command_4_ASICC, command_4_start_row, command_4_start_column + 32 * 3, BLACK,
                SLATE_GRAY);

  render_zh_cn_string(&self->lcd, command_exit_CN, 2, exit_command_start_row, exit_command_start_column, RED,
                      SLATE_GRAY);
}

static void draw_about_us(struct Ui *self) {
  render_string(&self->lcd, "By", 340, 420, BLACK, self->lcd.background_color);
  enum ZH_CH_CHARACTERS name_0[] = {ZENG, QING, JIE};
  render_zh_cn_string(&self->lcd, name_0, 3, 340, 460, BLACK, self->lcd.background_color);
  render_string(&self->lcd, "20220440333", 340, 560, BLACK, self->lcd.background_color);

  enum ZH_CH_CHARACTERS name_1[] = {WEN, JIA, JUN};
  render_zh_cn_string(&self->lcd, name_1, 3, 380, 460, BLACK, self->lcd.background_color);
  render_string(&self->lcd, "20220440333", 380, 560, BLACK, self->lcd.background_color);
  // TO-DO: display about us in the display panel
}

static void draw_prompt_window(struct Ui *self, enum ZH_CH_CHARACTERS *prompt, size_t length) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  // Store current screen
  self->lcd.copy_to_buffer(&self->lcd);

  // TO-DO: wrap the prompt text
  size_t prompt_length = length * 32;
  const size_t container_start_row = SCREEN_HEIGHT / 2 - 75;
  size_t container_start_column = SCREEN_WIDTH / 2 - (prompt_length + 10 + 25) / 2;

  const size_t container_height = 150;
  size_t container_width = prompt_length + 10 + 25;

  const size_t prompt_start_row = container_start_row + 20;
  const size_t prompt_start_column = container_start_column + 10;

  const size_t yes_start_row = container_start_row + 130;
  size_t yes_start_column = container_start_column + container_width / 4 - 16;

  const size_t no_start_row = container_start_row + 130;
  size_t no_start_column = container_start_column + container_width / 2 + container_width / 4 - 16;

  self->draw_window(self, container_height, container_width, container_start_row, container_start_column,
                    BLACK, STONE_300);
  render_zh_cn_string(&self->lcd, prompt, length, prompt_start_row, prompt_start_column, BLACK, STONE_300);
  render_zh_cn_character(&self->lcd, JA, yes_start_row, yes_start_column, RED, STONE_300);
  render_zh_cn_character(&self->lcd, NEIN, no_start_row, no_start_column, BLACK, STONE_300);
}

static void draw_time(struct Ui *self, size_t row, size_t column, enum COLOR color,
                      enum COLOR background_color) {
  if (self == NULL) {
    fprintf(stderr, "UI is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  time_t current_time = time(NULL);
  struct tm *time_info = localtime(&current_time);

  char time_string[12];
  strftime(time_string, 12, "%Y/%m/%d", time_info);

  render_string(&self->lcd, time_string, row, column, color, background_color);
}

void ui_new(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI constructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  lcd_new(&self->lcd);
  touch_new(&self->touch);
  self->prompt_window_width = 0;

  self->lcd.clear(&self->lcd);
  self->lcd.draw_background(&self->lcd, WHITE);

  self->draw_window = draw_window;
  self->draw_time = draw_time;
  self->draw_menu = draw_menu;
  self->draw_about_us = draw_about_us;
  self->draw_prompt_window = draw_prompt_window;
}

void ui_destructor(struct Ui *self) {
  if (self == NULL) {
    fprintf(stderr, "UI destructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  touch_destructor(&self->touch);
  lcd_destructor(&self->lcd);
}
