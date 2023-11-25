#pragma once

#include <Arduino.h>

#include "display.hpp"
#include "sensors.hpp"


void debug_display_print_illumination() {
  const int forward_left = forward_left_illumination_sensor.read();
  const int forward_right = forward_right_illumination_sensor.read();
  const int backward_left = backward_left_illumination_sensor.read();

  const int row_count = 2;
  const int col_length = 16 + 1;
  char buffer[row_count][col_length] = {};
  snprintf(buffer[0], col_length, "FL %*d  FR %*d", 4, forward_left, 4, forward_right);
  snprintf(buffer[1], col_length, "BL %*d", 4, backward_left);

  const char* const rows[] = { buffer[0], buffer[1] };
  display.print(rows, row_count);
}
