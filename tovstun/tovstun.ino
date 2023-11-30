#include <Arduino.h>

#include "sensors.hpp"
#include "motors.hpp"
#include "display.hpp"
#include "debug.hpp"

void setup() {
}

void loop() {
  debug_display_print_illumination();
  delay(100);
}
