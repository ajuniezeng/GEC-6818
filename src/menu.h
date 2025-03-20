#pragma once

#include "utils/touch.h"

enum MENU {
  INVALID,
  YES,
  CANCEL,
  TURN_ON_SLASH_OFF_LED1,
  TURN_ON_SLASH_OFF_LED2,
  TURN_ON_SLASH_OFF_LED3,
  TRUN_ON_SLASH_OFF_BEEP,
  EXIT,
  ABOUT,
};

enum MENU select_menu(struct Touch *touch);
