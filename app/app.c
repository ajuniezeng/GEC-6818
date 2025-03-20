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
    int led1 = 1;
    int led2 = 1;
    int led3 = 1;
    int beep = 1;

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

    switch (menu) {
      case (TURN_ON_SLASH_OFF_LED1):
        led1 = !led1;
        led_control(LED0, led1);
        break;
      case (TURN_ON_SLASH_OFF_LED2):
        led2 = !led2;
        led_control(LED1, led2);
        break;
      case (TURN_ON_SLASH_OFF_LED3):
        led3 = !led3;
        led_control(LED2, led3);
        break;
      case (TRUN_ON_SLASH_OFF_BEEP):
        beep = !beep;
        beep_control(beep);
        break;
      default:
        continue;
    }
  }
}
