
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"
//#include "display.hpp"
//#include "debug.hpp"

State state = State::Default;
unsigned long prev_time = 0;
unsigned long state_duration = 0; // For how long current state lasts
State next_state = State::Default;
State prev_state = State::Default;
StateData state_data{};

void setup()
{
  Serial.begin(9600);
  motors.attach();
  Serial.println("Hello world!");

  prev_time = millis();
}

void apply_movement()
{
  // TODO: Apply movement depending on current and next state
}

void tick_state_transition(const SensorsData &sensors)
{
  // TODO: Smth along the lines of the following
  /*
  if (state == State::AccelToSpeed) {
    if (state_duration > 500)
      next_state = State::HoldSpeed;
    if (sensors.is_forward_left_obstacle) {

    }
  } else if (state == State::HoldSpeed) {
    next_state = State::AccelToSpeed;
    state_data.speed = 69;
  }
  */
}

void tick_state_machine()
{
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

void loop()
{
  prev_state = state;
  //  tick_state_machine();
  //  debug_display_print_illumination();

  // if there is something in the front: accel + positive speed
  if ((forward_left_ultrasonic.read() <= 20 && forward_left_ultrasonic.read() != 0) ||
      (forward_right_ultrasonic.read() <= 20 && forward_right_ultrasonic.read() != 0)) {
    state = State::AccelToSpeed;
    state_data.speed = 10;
    // if there is nothing at front or sideways, accel + negative speed
  }
  else if (forward_left_ultrasonic.read() > 20 && forward_right_ultrasonic.read() > 20) {
    state = State::AccelToSpeed;
    state_data.speed = -10;
  }

  // THAT MUST BE LAST IFS: stop if we see white line
  if (state_data.speed > 0 && forward_right_illumination_sensor.collides() ||
      forward_left_illumination_sensor.collides()) {
    state = State::Stop;
  }
  if (state_data.speed < 0 && backward_left_illumination_sensor.collides()) {
    state = State::Stop;
  }
  // State: accelerating
  if (state == State::AccelToSpeed) {
    // if prev state was also accelerating
    if (prev_state == State::AccelToSpeed) {
      // if previously we were acclerating to the same direction
      if (motors.get_unite_speed() >= 0 && state_data.speed >= 0 ||
          motors.get_unite_speed() < 0 && state_data.speed < 0) {
        // just continue to accelerate
        // if we need to accelerate more
        if (abs(motors.get_unite_speed()) < abs(state_data.speed)) {
          if (state_data.speed > 0)
            motors.move_forward(motors.get_unite_speed() + 3);
          else if (state_data.speed < 0)
            motors.move_backward(abs(motors.get_unite_speed()) + 3);
          Serial.println("increasing the speed");
        }
        Serial.println("speed is ok");
        // ok, we are accelereting, but motors are in different state
      }
      else {
        Serial.println("speed is not unite");
        Serial.print("Motor: ");
        Serial.println(motors.get_unite_speed());
        if (state_data.speed > 0) {
          motors.prepare_forward();
          motors.move_forward(3);
          Serial.println("Preparing moving forward");
        }
        else {
          motors.prepare_backward();
          motors.move_backward(3);
          Serial.println("Preparing moving backward");
        }
      }
    }
    else {
      if (state_data.speed > 0) {
        motors.prepare_forward();
        motors.move_forward(3);
        Serial.println("Preparing moving forward");
      }
      else {
        motors.prepare_backward();
        motors.move_backward(3);
        Serial.println("Preparing moving backward");
      }
    }
  }
  else if (state == State::DeccelToSpeed) {
    if (prev_state == State::DeccelToSpeed) {
      if (motors.get_unite_speed() >= 0 && state_data.speed >= 0 ||
          motors.get_unite_speed() < 0 && state_data.speed < 0) {
        if (abs(motors.get_unite_speed()) > abs(state_data.speed)) {
          if (state_data.speed > 0)
            motors.move_forward(motors.get_unite_speed() - 3);
          else if (state_data.speed < 0)
            motors.move_backward(abs(motors.get_unite_speed()) - 3);
          Serial.println("decreasing the speed");
        }
        Serial.println("speed is ok");
      }
      else {
        Serial.println("speed is not unite");
        Serial.print("Motor: ");
        Serial.println(motors.get_unite_speed());
        if (state_data.speed > 0) {
          motors.prepare_forward();
          motors.move_forward(3);
          Serial.println("Preparing moving forward");
        }
        else {
          motors.prepare_backward();
          motors.move_backward(3);
          Serial.println("Preparing moving backward");
        }
      }
    }
    else {
      if (state_data.speed > 0) {
        motors.prepare_forward();
        motors.move_forward(3);
        Serial.println("Preparing moving forward");
      }
      else {
        motors.prepare_backward();
        motors.move_backward(3);
        Serial.println("Preparing moving backward");
      }
    }
  }
  delay(1000);
}
