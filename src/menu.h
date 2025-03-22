#pragma once

#include "ui.h"

enum MENU {
  INVALID,
  YES,
  CANCEL,
  LED_CONTROL,
  TEMPERATURE_AND_HUMIDITY_DETECTION,
  SMOKE_DETECTION,
  SELECT_LED0,
  SELECT_LED1,
  SELECT_LED2,
  SELECT_LED3,
  EXIT,
};

enum MENU select_menu_led_control(struct Ui *ui, struct LedStatus *led_status);
enum MENU prompt_window(struct Ui *ui);
enum MENU select_menu_temperature_humidity_detection(struct Ui *ui);
enum MENU select_menu_smoke_detection(struct Ui *ui);
