#pragma once
#include <Arduino.h>
#include <Servo.h>

// motors
const int left_motor_pin = 9;
const int right_motor_pin = 10;

// servo for the flag
const int flag_servo_pin = 11;

struct Flag {
  Servo flag;

  static constexpr int RAISED = 120;
  static constexpr int LOWERED = 50;

  Flag() { flag.attach(flag_servo_pin); }

  void raise() { flag.write(RAISED); }

  void lower() { flag.write(LOWERED); }
};

extern Flag flag;

struct Motors {
  static constexpr unsigned long prepare_write_duration = 100;
  void attach()
  {
    l_motor.attach(left_motor_pin);
    r_motor.attach(right_motor_pin);
    write_for(90, 2000);
  }
  void prepare_forward()
  {
    if (l_prev_speed >= 90 && r_prev_speed >= 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed < 90)
        l_motor.write(90);
      if (r_prev_speed < 90)
        r_motor.write(90);
    }
  }
  // я навіть не буду намагатися писати коментарі англійською
  // в цьому коді правди нема, а баги є
  // воно таке страшне тільки тому, щоб перемикати назад не за 12с, а за 6(а то і менше)
  void prepare_backward()
  {
    if (l_prev_speed < 90 && r_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
  }

  void move(int speed)
  {
    int actual_speed = speed + 90;
    if (speed == 0) {
      write_for(90, 20);
    }
    else if (speed > 0) {
      prepare_forward();
      write_for(actual_speed, 40);
    }
    else if (speed < 0) {
      prepare_backward();
      write_for(actual_speed, 40);
    }
    l_prev_speed = actual_speed;
    r_prev_speed = actual_speed;
  }
  void prepare_left_forward_right_backward()
  {
    if (l_prev_speed >= 90 && r_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
  }
  void prepare_left_backward_right_forward()
  {
    if (r_prev_speed >= 90 && l_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
    }
  }
  void prepare_left()
  {
    if (r_prev_speed >= 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (r_prev_speed < 90)
        r_motor.write(90);
    }
  }
  void prepare_right()
  {
    if (l_prev_speed >= 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration) {
      if (l_prev_speed < 90)
        l_motor.write(90);
    }
  }
  void write_for(int speed, unsigned long miliseconds)
  {
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds) {
      l_motor.write(speed);
      r_motor.write(speed);
    }
  }
  void write_for_l(int speed, unsigned long miliseconds)
  {
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds) {
      l_motor.write(speed);
    }
  }
  void write_for_r(int speed, unsigned long miliseconds)
  {
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds) {
      r_motor.write(speed);
    }
  }
  void rotate_left(int speed)
  {
    prepare_left();
    write_for_r(speed, 50);
  }
  void rotate_right(int speed)
  {
    prepare_right();
    write_for_l(speed, 50);
  }
  // ASSUME: speed for motors is equal
  int get_unite_speed() { return l_prev_speed - 90; }
  int l_prev_speed = 90, r_prev_speed = 90;
  Servo l_motor, r_motor;
};

extern Motors motors;
