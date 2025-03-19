#include "touch.h"

#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static enum MOVE get_move(struct Touch *self) {
  if (self == NULL || self->device == -1) {
    fprintf(stderr, "Touch is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  struct input_event event;
  int start_x = 0, start_y = 0;
  while (1) {
    int status = read(self->device, &event, sizeof(struct input_event));
    if (status == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (event.type == EV_ABS) {
      if (event.code == ABS_X) self->x = event.value * 800 / 1024;
      if (event.code == ABS_Y) self->y = event.value * 480 / 600;
    } else if (event.type == EV_KEY && event.code == BTN_TOUCH) {
      if (event.value == 1) {
        start_x = self->x;
        start_y = self->y;
      }
      if (event.value == 0) {
        const int delta = 50;
        if (self->x > start_x && self->x - start_x >= delta) return RIGHT;
        if (start_x > self->x && start_x - self->x >= delta) return LEFT;
        if (start_y > self->y && start_y - self->y >= delta) return UP;
        if (self->y > start_y && self->y - start_y >= delta) return DOWN;
        return TAP;
      }
    }
  }
  return NONE;
}

void touch_new(struct Touch *self) {
  if (self == NULL) {
    fprintf(stderr, "Touch constructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  self->device = open(TOUCH_DEVICE_PATH, O_RDONLY);
  if (self->device == -1) {
    perror("Failed to open touch device");
    exit(EXIT_FAILURE);
  }

  self->get_move = get_move;
}

void touch_destructor(struct Touch *self) {
  if (self == NULL || self->device == -1) {
    fprintf(stderr, "Touch destructs on NULL\n");
    exit(EXIT_FAILURE);
  }

  close(self->device);
}
