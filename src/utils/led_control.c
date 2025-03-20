#include "led_control.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

void beep_control(int value) {
  int device = open(LED_BEEP_PATH, O_RDWR);

  if (device == -1) {
    perror("Failed to open beep device");
    exit(EXIT_FAILURE);
  }

  if (write(device, &value, sizeof(value)) == -1) {
    perror("Failed to write beep value");
    exit(EXIT_FAILURE);
  }

  close(device);
}

void led_control(enum LED led, int value) {
  char *led_path;
  switch (led) {
    case LED0:
      led_path = LED1_PATH;
      break;
    case LED1:
      led_path = LED2_PATH;
      break;
    case LED2:
      led_path = LED3_PATH;
      break;
    case LED3:
      led_path = LED4_PATH;
      break;
    case LED_ALL:
      led_path = LED_ALL_PATH;
      break;
    default:
      fprintf(stderr, "Invalid LED\n");
      exit(EXIT_FAILURE);
  }

  int device = open(led_path, O_RDWR);

  if (device == -1) {
    perror("Failed to open led device");
    exit(EXIT_FAILURE);
  }

  if (write(device, &value, sizeof(led)) == -1) {
    perror("Failed to write led value");
    exit(EXIT_FAILURE);
  }

  close(device);
}
