#include "app.h"

#include <stdio.h>
#include <unistd.h>

#include "menu.h"
#include "ui.h"
#include "utils/led_control.h"

void start(void) {
  struct Ui ui;
  ui_new(&ui);

  ui.draw_menu(&ui);

  int led1 = 1;
  int led2 = 1;
  int led3 = 1;

  while (1) {
    enum MENU menu;

    while (1) {
      menu = select_menu(&ui.touch);
      if (menu == INVALID)
        continue;
      else
        break;
    }

    printf("%d\n", menu);

    if (menu == EXIT) {
      // enum ZH_CH_CHARACTERS prompt[] = {YOU, QUE, DING, WANT, TUI, CHU, MA, QUESTION};
      // ui.draw_prompt_window(&ui, prompt, 8);
      led_control(LED_ALL, 0);
      sleep(1);
      beep_control(1);
      sleep(1);
      beep_control(0);
      sleep(1);
      led_control(LED_ALL, 1);
      break;
    }

    if (TURN_ON_SLASH_OFF_LED1) {
      if (!led1) {
        led_control(LED0, 1);
        led1 = 1;
      } else {
        led_control(LED0, 0);
        led1 = 0;
      }
    }
    if (TURN_ON_SLASH_OFF_LED2) {
      if (!led2) {
        led_control(LED1, 1);
        led2 = 1;
      } else {
        led_control(LED1, 0);
        led2 = 0;
      }
    }
    if (TURN_ON_SLASH_OFF_LED3) {
      if (!led3) {
        led_control(LED2, 1);
        led3 = 1;
      } else {
        led_control(LED2, 0);
        led3 = 0;
      }
    }
    if (TRUN_ON_SLASH_OFF_BEEP) {
      beep_control(1);
      sleep(1);
      beep_control(0);
    }
  }
  ui.lcd.draw_background(&ui.lcd, BLACK);
  ui_destructor(&ui);
}
