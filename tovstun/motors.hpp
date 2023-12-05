#pragma once
#include <Arduino.h>
#include <Servo.h>

// motors
const int left_motor_pin = 9;
const int right_motor_pin = 10;

// servo for the flag
const int flag_servo_pin = 11;

void initialize_motors();
/**
  \param motor the motor to rotate
  \param speed the speed in [0, 90]
 */

struct Motor {
  // prev speed in range 0 - 180 (natrual speed for the motor)
  int prev_speed;
  int pin;
  Servo motor;
  // duration of useful movement
  constexpr static unsigned move_duration{ 500 };
  // duration of stoping
  constexpr static unsigned stop_duration{ 50 };
  Motor(int pin) : prev_speed{ 90 }, pin{pin} {}
  void attach()
  {
    motor.attach(pin);
    write_for(90, 1000);
  }

  // prepare motor to move forward (if it moved backwards last time)
  void prepare_move_forward() {
    if (prev_speed < 90)
      write_for(90, 500);
  }

  /**
   * \param speed = speed from 0 to 90
  */
  void move_forward(int speed) {
    speed += 90;
    prepare_move_forward();
    write_for(speed, move_duration);
    prev_speed = speed;
    write_for(90, stop_duration);
  }
  // prepare motor to move backward (if it moved forward last time)
  void prepare_move_backward() {
    if (prev_speed >= 90) {
      write_for(90, 200);
      write_for(0, 200);
      write_for(90, 200);
      write_for(87, 200);
      write_for(90, 200);
      write_for(87, 200);
    }
    prev_speed = 87;
  }

  /**
   * \param speed = speed from 0 to 90
  */
  void move_backward(int speed) {
    speed = 90 - speed;
    prepare_move_backward();
    Serial.println("moving backwards");
    write_for(speed, move_duration);
    prev_speed = speed;
    // delay(1000);
    write_for(90, stop_duration);
  }

  /**
   * write speed value for selected time;
   */
  void write_for(int speed, unsigned long miliseconds) {
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds)
      motor.write(speed);
  }
};

extern Motor left_motor;
extern Motor right_motor;


