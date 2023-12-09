
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
  motors.write_for(90, 2000);
  pinMode(red_button_pin, INPUT);
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
  Serial.print("TS: ");
  Serial.println(state_data.speed);
  Serial.print("CS: ");
  Serial.println(motors.get_unite_speed());
  Serial.println("---------------------------");
  prev_state = state;
  Serial.print("FL: ");
  Serial.println(forward_left_ultrasonic.read());
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

  // THAT MUST BE BEFORE LAST IF: stop if we see white line
  if (state_data.speed > 0 && forward_right_illumination_sensor.collides() ||
      forward_left_illumination_sensor.collides()) {
    state = State::Stop;
  }
  if (state_data.speed < 0 && backward_left_illumination_sensor.collides()) {
    state = State::Stop;
  }
  //CHANGE IT: && false is for debug *ONLY*
  if (digitalRead(red_button_pin)) {
    Serial.println("RED BUTTON PRESSED");
    state = State::RedButtonStopped;
    prev_state = State::RedButtonStopped;
  }
  if (prev_state == State::RedButtonStopped)
    state = State::RedButtonStopped;
  //////////////////////////////////DEBUG///////////////////////////////////
  // if (forward_left_ultrasonic.read() < 20) {
  //   state = State::AccelToSpeed;
  //   state_data.speed = 15;
  // }
  // else {
  //   state = State::DeccelToSpeed;
  //   // prev_state = state;
  //   state_data.speed = 0;
  // }
  //////////////////////////////////DEBUG///////////////////////////////////
  Serial.print("STATE: ");
  Serial.println(static_cast<int>(state));
  Serial.print("PREV STATE: ");
  Serial.println(static_cast<int>(state));
  // State: accelerating
  if (state == State::AccelToSpeed) {
    Serial.println("ACCEL");

    if (state_data.speed >= 0 && motors.get_unite_speed() < state_data.speed)
      motors.move(motors.get_unite_speed() + 3);
    else if (state_data.speed <= 0 && motors.get_unite_speed() > state_data.speed)
      motors.move(motors.get_unite_speed() - 3);
  }
  else if (state == State::DeccelToSpeed) {
    Serial.println("DECCEL");

    if (state_data.speed >= 0 && motors.get_unite_speed() > state_data.speed)
      motors.move(max(0, motors.get_unite_speed() - 3));
    else if (state_data.speed <= 0 && motors.get_unite_speed() < state_data.speed)
      motors.move(min(0, motors.get_unite_speed() + 3));
  }
  else if (state == State::Stop || state == State::RedButtonStopped) {
    motors.move(0);
  }
  Serial.print("TS: ");
  Serial.println(state_data.speed);
  Serial.print("CS: ");
  Serial.println(motors.get_unite_speed());

  delay(500);
}
