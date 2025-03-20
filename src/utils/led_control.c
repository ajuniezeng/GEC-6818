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
  char *led_path = LED_PATH;
  char *device_name;
  switch (led) {
    case LED0:
      device_name = "led_d7";
      strcat(led_path, device_name);
      break;
    case LED1:
      device_name = "led_d8";
      strcat(led_path, device_name);
      break;
    case LED2:
      device_name = "led_d9";
      strcat(led_path, device_name);
      break;
    case LED3:
      device_name = "led_d10";
      strcat(led_path, device_name);
      break;
    case LED_ALL:
      led_path = "led_d11";
      strcat(led_path, device_name);
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
