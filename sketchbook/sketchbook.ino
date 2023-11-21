#include <Servo.h>
#include <Ultrasonic.h>

//ultrasonic sernsors
const int forward_left_ultrasonic_trig_pin = 0;
const int forward_left_ultrasonic_echo_pin = 1;
Ultrasonic forward_left_ultrasonic(forward_left_ultrasonic_trig_pin, forward_left_ultrasonic_echo_pin);

const int forward_right_ultrasonic_trig_pin = 2;
const int forward_right_ultrasonic_echo_pin = 3;
Ultrasonic forward_right_ultrasonic(forward_right_ultrasonic_trig_pin, forward_right_ultrasonic_echo_pin);

const int left_ultrasonic_trig_pin = 4;
const int left_ultrasonic_echo_pin = 5;
Ultrasonic left_ultrasonic(left_ultrasonic_trig_pin, left_ultrasonic_echo_pin);

const int right_ultrasonic_trig_pin = 6;
const int right_ultrasonic_echo_pin = 7;
Ultrasonic right_ultrasonic(right_ultrasonic_trig_pin, right_ultrasonic_echo_pin);


// illumination sensors (ANALOG)
const int forward_left_illumination_pin = 0;
const int forward_right_illumination_pin = 1;
const int backward_illumination_pin = 2;


// motors
const int left_motor_pin = 9;
const int right_motor_pin = 10;


//servo for the flag
const int flag_servo_pin = 11;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
