#pragma once

#define LED_BEEP_PATH "/sys/kernel/gec_ctrl/beep"
#define LED1_PATH "/sys/kernel/gec_ctrl/led_d7"
#define LED2_PATH "/sys/kernel/gec_ctrl/led_d8"
#define LED3_PATH "/sys/kernel/gec_ctrl/led_d9"
#define LED4_PATH "/sys/kernel/gec_ctrl/led_d10"
#define LED_ALL_PATH "/sys/kernel/gec_ctrl/led_all"

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
