
#include "sensors.hpp"
//#include "motors.hpp"
//#include "display.hpp"
//#include "debug.hpp"

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
}
int speed = 0;
void loop() {
  Serial.println("Hello world!");
  // debug_display_print_illumination();
  // if(forward_left_ultrasonic.read() <= 20 || forward_right_ultrasonic.read() <= 20) {
  //   if(forward_left_illumination_sensor.collides() || forward_left_illumination_sensor.collides()) {
  //     speed = 0;
  //     move_forward(0);
  //   } else{
  //     if(speed < 30)
  //       speed += 5;
  //     move_forward(speed);
  //   }
  // } else if(forward_left_ultrasonic.read() > 20 || forward_right_ultrasonic.read() > 20) {
  //   if(backward_left_illumination_sensor.collides()) {
  //     speed = 0;
  //     move_backward(0);
  //   } else{
  //     if(speed < 30)
  //       speed += 5;
  //     move_backward(speed);
  //   }
  // }
 // Serial.println(forward_left_ultrasonic.read());
  Serial.println(forward_right_ultrasonic.read());
  delay(1000);
}
