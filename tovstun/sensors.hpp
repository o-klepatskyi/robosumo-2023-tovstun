#pragma once

#include <Arduino.h>
#include <Ultrasonic.h>

// ultrasonic sensors
const int forward_left_ultrasonic_trig_pin = 12;
const int forward_left_ultrasonic_echo_pin = 13;
extern Ultrasonic forward_left_ultrasonic;

const int forward_right_ultrasonic_trig_pin = 2;
const int forward_right_ultrasonic_echo_pin = 3;
extern Ultrasonic forward_right_ultrasonic;

const int left_ultrasonic_trig_pin = 4;
const int left_ultrasonic_echo_pin = 5;
extern Ultrasonic left_ultrasonic;

const int right_ultrasonic_trig_pin = 6;
const int right_ultrasonic_echo_pin = 7;
extern Ultrasonic right_ultrasonic;

// illumination sensors (ANALOG)
const int forward_left_illumination_pin = A0;
const int forward_right_illumination_pin = A1;
const int backward_illumination_pin = A2;

struct Illumination_sensor {
  static constexpr int threshold = 50;
  int pin;

  Illumination_sensor(int pin)
    : pin{ pin } {
    pinMode(pin, INPUT);
  }

  /**
   * Is there an obstacle?
   */
  bool collides() {
    return read() >= threshold;
  }

  int read() {
    return analogRead(pin);
  }
};

extern Illumination_sensor forward_left_illumination_sensor;
extern Illumination_sensor forward_right_illumination_sensor;
extern Illumination_sensor backward_left_illumination_sensor;