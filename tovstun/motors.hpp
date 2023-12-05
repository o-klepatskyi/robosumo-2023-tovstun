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

  Flag() {
    flag.attach(flag_servo_pin);
  }

  void raise() {
    flag.write(RAISED);
  }

  void lower() {
    flag.write(LOWERED);
  }
};

extern Flag flag;


struct Motors {

  void attach() {
    l_motor.attach(left_motor_pin);
    r_motor.attach(right_motor_pin);
  }
  void prepare_forward() {
    if (l_prev_speed >= 90 && r_prev_speed >= 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed < 90)
        l_motor.write(90);
      if (r_prev_speed < 90)
        r_motor.write(90);
    }
  }
  // я навіть не буду намагатися писати коментарі англійською
  // в цьому коді правди нема, а баги є
  // воно таке страшне тільки тому, щоб перемикати назад не за 12с, а за 6(а то і менше)
  void prepare_backward() {
    if (l_prev_speed < 90 && r_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(0);
      if (r_prev_speed >= 90)
        r_motor.write(0);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
  }

  void move_forward(int speed) {
    prepare_forward();
    speed += 90;
    l_prev_speed = speed;
    r_prev_speed = speed;
    write_for(speed, 20);
  }
  void move_backward(int speed) {
    prepare_backward();
    speed = 90 - speed;
    l_prev_speed = speed;
    r_prev_speed = speed;
    write_for(speed, 20);
  }
  prepare_left_forward_right_backward() {
    if (l_prev_speed >= 90 && r_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed < 90)
        l_motor.write(90);
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (r_prev_speed >= 90)
        r_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (r_prev_speed >= 90)
        r_motor.write(87);
    }
  }
    prepare_left_backward_right_forward() {
    if (r_prev_speed >= 90 && l_prev_speed < 90)
      return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (r_prev_speed < 90)
        r_motor.write(90);
      if (l_prev_speed >= 90)
        l_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(90);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= 500) {
      if (l_prev_speed >= 90)
        l_motor.write(87);
    }
  }

  void write_for(int speed, unsigned long miliseconds) {
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds) {
      l_motor.write(speed);
      r_motor.write(speed);
    }
  }

  int l_prev_speed = 90,
      r_prev_speed = 90;
  Servo l_motor,
    r_motor;
};

extern Motors motors;
