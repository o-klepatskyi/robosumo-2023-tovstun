#pragma once

#include <Arduino.h>
#include <Ultrasonic.h>

// ultrasonic sensors
const int forward_left_ultrasonic_trig_pin = 0;
const int forward_left_ultrasonic_echo_pin = 1;
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
const int forward_left_illumination_pin = 0;
const int forward_right_illumination_pin = 1;
const int backward_illumination_pin = 2;
