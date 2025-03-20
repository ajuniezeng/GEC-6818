#pragma once

#define LED_BEEP_PATH "/sys/kernel/gec_ctrl/beep"
#define LED_PATH "/sys/kernel/gec_ctrl/"

enum LED {
  LED0,
  LED1,
  LED2,
  LED3,
  LED_ALL,
};

/// Control the beep
///
/// value = 1: turn on the beep
///
/// value = 0: turn off the beep
void beep_control(int value);

/// Control the LED
///
/// led: the LED to control
///
/// value = 1: turn on the LED
///
/// value = 0: turn off the LED
void led_control(enum LED led, int value);
