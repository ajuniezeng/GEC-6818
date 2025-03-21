#include "app.h"

#include <stdio.h>
#include <unistd.h>

#include "menu.h"
#include "ui.h"
#include "utils/led_control.h"

void start(void) {
  struct Ui ui;
  ui_new(&ui);

  
  int led0 = 1;
  int led1 = 1;
  int led2 = 1;
  int led3 = 1;
  
  while (1) {
    enum MENU menu;
    
    while (1) {
      enum UiType current_ui = ui.current_ui;
      
      switch (current_ui) {
        case SELECT_MENU_LED_CONTROL:
        ui.draw_menu_led_control(&ui);
          menu = select_menu_led_control(&ui.touch);
          break;
        case PROMPT_WINDOW:
          menu = prompt_window(&ui);
          break;
        // case SELECT_MENU_SMOKE_DETECTION:
        // 
        //   break;
        case SELECT_MENU_TEMPERATURE_HUMIDITY_DETECTION:
          ui.draw_menu_temperature_humidity_detection(&ui);
          menu = select_menu_main_options(&ui.touch);
          break;
        default:
          menu = INVALID;
          break;
      }

      if (menu == INVALID)
        continue;
      else
        break;
    }

    printf("menu = %d\n", menu);

    if (menu == EXIT) {
      enum ZH_CH_CHARACTERS prompt[] = {YOU, QUE, DING, WANT, TUI, CHU, MA, QUESTION};
      ui.draw_prompt_window(&ui, prompt, 8);
      continue;
    }

    if (menu == YES) {
      break;
    }

    if (menu == CANCEL) {
      switch (ui.previous_ui) {
        case SELECT_MENU_LED_CONTROL:
          ui.current_ui = SELECT_MENU_LED_CONTROL;
          break;
        default:
          perror("Invalid previous UI");
          break;
      }
      break;
    }

    if (menu == SELECT_LED0) {
      if (!led0) {
        led_control(LED0, 1);
        led0 = 1;
      } else {
        led_control(LED0, 0);
        led0 = 0;
      }
      ui.draw_led_status(&ui, LED0, led0);
      continue;
    }

    if (menu == SELECT_LED1) {
      if (!led1) {
        led_control(LED1, 1);
        led1 = 1;
      } else {
        led_control(LED1, 0);
        led1 = 0;
      }
      ui.draw_led_status(&ui, LED1, led1);
      continue;
    }

    if (menu == SELECT_LED2) {
      if (!led2) {
        led_control(LED2, 1);
        led2 = 1;
      } else {
        led_control(LED2, 0);
        led2 = 0;
      }
      ui.draw_led_status(&ui, LED2, led2);
      continue;
    }

    if (menu == SELECT_LED3) {
      if (!led3) {
        led_control(LED3, 1);
        led3 = 1;
      } else {
        led_control(LED3, 0);
        led3 = 0;
      }
      ui.draw_led_status(&ui, LED3, led3);
      continue;
    }
  }

  ui.lcd.draw_background(&ui.lcd, BLACK);
  ui_destructor(&ui);
}
