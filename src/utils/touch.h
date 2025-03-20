#pragma once

#define TOUCH_DEVICE_PATH "/dev/input/event0"

// #include <linux/input.h>
#include <stddef.h>

enum MOVE { UP, DOWN, LEFT, RIGHT, TAP, NONE };

struct Touch {
  int device;
  int x, y;

  /// Get the move from the touch screen
  enum MOVE (*get_move)(struct Touch *self);
};

/// Initialize the touch screen
void touch_new(struct Touch *self);

/// Release the resources of the touch screen
void touch_destructor(struct Touch *self);
