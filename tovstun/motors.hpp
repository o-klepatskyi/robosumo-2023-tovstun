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
  Motor(int pin): prev_speed{90} 
  {
    motor.attach(pin);
    write_for(90, 1000);
  }
  /**
   * \param speed = speed from 0 to 90
  */
  void move_forward(int speed)
  {
    speed += 90;
    if(prev_speed < 90)
      write_for(90, 500);
    motor.write(speed);
    prev_speed = speed;
  }
  /**
   * \param speed = speed from 0 to 90
  */
  void move_backward(int speed)
  {
    speed = 90 - speed;
    if(prev_speed >= 90) {
      write_for(90, 500);
      write_for(0, 500);
      write_for(90, 500);
      write_for(87, 500);
      write_for(90, 500);
      write_for(87, 500);
    }
    motor.write(speed);
    prev_speed = speed;
  }
  /**
   * write speed value for selected time;
   */
  void write_for(int speed, unsigned long miliseconds) 
  {
    unsigned long prevMillis = millis();
    while(millis() - prevMillis <= miliseconds)
      motor.write(speed);
  }


  // prev speed in range 0 - 180 (natrual speed for the motor)
  int prev_speed;
  Servo motor; 

};

extern Motor left_motor;
extern Motor right_motor;

struct Flag {
  static constexpr int RAISED = 120;
  static constexpr int LOWERED = 50;
  Flag() {
    flag.attach(flag_servo_pin);
  }
  void raise()
  {
    flag.write(RAISED);
  }
  void lower()
  {
    flag.write(LOWERED);
  }
  Servo flag;
};

extern Flag flag;