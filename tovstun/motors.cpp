#include "motors.hpp"

Motor left_motor{ left_motor_pin };
Motor right_motor{ right_motor_pin };

void move_forward(int speed) {
  left_motor.prepare_move_forward();
  right_motor.prepare_move_forward();

  left_motor.move_forward(speed);
  right_motor.move_forward(speed);
}

void move_backward(int speed) {
  left_motor.prepare_move_backward();
  right_motor.prepare_move_backward();

  left_motor.move_backward(speed);
  right_motor.move_backward(speed);
}

void stop() {
  left_motor.move_forward(0);
  right_motor.move_forward(0);
}

Flag flag;