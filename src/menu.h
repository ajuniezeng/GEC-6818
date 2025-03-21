#pragma once

#include "ui.h"
#include "utils/touch.h"

enum MENU {
  INVALID,
  YES,
  CANCEL,
  LIGHT_CONTROL,
  TEMPERATURE_AND_HUMIDITY_DETECTION,
  SMOKE_DETECTION,
  SELECT_LED0,
  SELECT_LED1,
  SELECT_LED2,
  SELECT_LED3,
  EXIT,
};

enum MENU select_menu_led_control(struct Touch *touch);
enum MENU prompt_window(struct Ui *ui);
enum MENU select_menu_main_options(struct Touch *touch);
