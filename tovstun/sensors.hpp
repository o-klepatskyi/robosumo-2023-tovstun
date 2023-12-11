#pragma once

#include <Arduino.h>
#include <Ultrasonic.h>

// ultrasonic sensors
const int forward_left_ultrasonic_trig_pin = 4;
const int forward_left_ultrasonic_echo_pin = 5;
extern Ultrasonic front_left_ultrasonic;

const int forward_right_ultrasonic_trig_pin = 3;
const int forward_right_ultrasonic_echo_pin = 2;
extern Ultrasonic front_right_ultrasonic;

const int left_ultrasonic_trig_pin = 8;
const int left_ultrasonic_echo_pin = 11;
extern Ultrasonic left_ultrasonic;

const int right_ultrasonic_trig_pin = 6;
const int right_ultrasonic_echo_pin = 7;
extern Ultrasonic right_ultrasonic;

// illumination sensors (ANALOG)
const int forward_left_illumination_pin = A0;
const int forward_right_illumination_pin = A1;
const int backward_illumination_pin = A2;

const int red_button_pin = 13;

struct Illumination_sensor {
  int threshold = 50;
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

extern Illumination_sensor front_left_illumination_sensor;
extern Illumination_sensor front_right_illumination_sensor;
extern Illumination_sensor back_illumination_sensor;

struct SensorsData {
  bool is_fl_obstacle; // detected line on front left
  bool is_fr_obstacle; // detected line on front right
  bool is_back_obstacle;

  int fl_us_value; // front left ultrasonic value
  int fr_us_value; // front right ultrasonic value
  int l_us_value; // left ultrasonic value
  int r_us_value; // right ultrasonic value

  static SensorsData read() {
    return SensorsData {
      front_left_illumination_sensor.collides(),
      front_right_illumination_sensor.collides(),
      back_illumination_sensor.collides(),
      front_left_ultrasonic.read(),
      front_right_ultrasonic.read(),
      left_ultrasonic.read(),
      right_ultrasonic.read()
    };
  }

};
