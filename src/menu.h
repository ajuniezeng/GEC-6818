#pragma once

#include "utils/lcd_control.h"
#include "utils/touch.h"

enum MENU {
  INVALID,
  EXIT,
  ABOUT,
};

void render_menu_ui(struct LCD *lcd);
enum MENU select_menu(struct Touch *touch);
