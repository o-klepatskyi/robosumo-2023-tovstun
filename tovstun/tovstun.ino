
#include "sensors.hpp"
#include "motors.hpp"
#include "state.hpp"
//#include "display.hpp"
//#include "debug.hpp"

State state = State::Default;
unsigned long prev_time = 0;
unsigned long state_duration = 0; // For how long current state lasts

State next_state = State::Default;
StateData next_state_data{};

void setup() {
  Serial.begin(9600);
  motors.attach();
  Serial.println("Hello world!");

  prev_time = millis();
}

void apply_movement() {
  // TODO: Apply movement depending on current and next state
}

void tick_state_transition(const SensorsData& sensors) {
  // TODO: Smth along the lines of the following
  /*
  if (state == State::AccelToSpeed) {
    if (state_duration > 500)
      next_state = State::HoldSpeed;
    if (sensors.is_forward_left_obstacle) {
      
    }
  } else if (state == State::HoldSpeed) {
    next_state = State::AccelToSpeed;
    next_state_data.speed = 69;
  }
  */
}

void tick_state_machine() {
  const unsigned long curr_time = millis();
  const unsigned long dt = curr_time - prev_time;
  prev_time = curr_time;
  state_duration += dt;
  
  const SensorsData sensors = SensorsData::read();

  tick_state_transition(sensors);
  apply_movement();

  if (state == next_state)
    return;

  state = next_state;
  state_duration = 0;
}

// int speed = 0;

void loop() {
  Serial.println("Hello world!");

  tick_state_machine();  

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
  
  // Serial.println(forward_right_ultrasonic.read());
  // delay(100);
}
