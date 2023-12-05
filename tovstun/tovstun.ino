
#include "sensors.hpp"
#include "motors.hpp"
//#include "display.hpp"
#include "debug.hpp"

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  left_motor.attach();
  right_motor.attach();
}
int speed = 0;
void loop() {
  if(forward_left_ultrasonic.read() <= 20 || forward_right_ultrasonic.read() <= 20) {
    if(forward_left_illumination_sensor.collides() || forward_left_illumination_sensor.collides()) {
      speed = 0;
      move_forward(0);
    } else{
      if(speed < 30)
        speed += 5;
      move_forward(speed);
    }
  } else if(forward_left_ultrasonic.read() > 20 || forward_right_ultrasonic.read() > 20) {
    if(backward_left_illumination_sensor.collides()) {
      speed = 0;
      move_backward(0);
    } else{
      if(speed < 30)
        speed += 5;
      move_backward(speed);
    }
  }
debug_serial_sensors();
delay(100);
}
