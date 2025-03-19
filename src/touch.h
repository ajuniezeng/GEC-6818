#pragma once

#define TOUCH_DEVICE_PATH "/dev/input/event0"

// #include <linux/input.h>
#include <stddef.h>

enum MOVE { UP, DOWN, LEFT, RIGHT, TAP, NONE };

struct Touch {
  int device;
  int x, y;

  enum MOVE (*get_move)(struct Touch *self);
};

void touch_new(struct Touch *self);
void touch_destructor(struct Touch *self);
