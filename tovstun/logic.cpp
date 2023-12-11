#include "logic.hpp"
#include "sensors.hpp"
#include "state.hpp"
#include "motors.hpp"
#include <Arduino.h> // Serial.println

static State state = State::Default;
static State next_state = State::Default;
static State prev_state = State::Default;

// static unsigned long prev_time = 0;
// static unsigned long state_duration = 0; // For how long current state lasts

static StateData state_data{};

// decide what state in next depending on the sensors values
void state_transition(const SensorsData& sensors);

// apply current movement state, i.e. send signals to motors
void apply_movement();

void debug_print();

void on_loop()
{
    // const unsigned long curr_time = millis();
    // const unsigned long dt = curr_time - prev_time;
    // prev_time = curr_time;
    // state_duration += dt;

    prev_state = state;
    const SensorsData sensors = SensorsData::read();

    state_transition(sensors);
    apply_movement();

    // if (state == next_state)
        // return;

    // state = next_state;
    // state_duration = 0;

    // SensorsData sensors = SensorsData::read();
    // state_transition(sensors);
    // apply_movement();
}

void state_transition(const SensorsData& sensors)
{
    // if there is something in the front: accel + positive speed
    if ((forward_left_ultrasonic.read() <= 20 && forward_left_ultrasonic.read() != 0) ||
        (forward_right_ultrasonic.read() <= 20 && forward_right_ultrasonic.read() != 0))
    {
        state = State::AccelToSpeed;
        state_data.speed = 10;
        // if there is nothing at front or sideways, accel + negative speed
    }
    else if (forward_left_ultrasonic.read() > 20 && forward_right_ultrasonic.read() > 20)
    {
        state = State::AccelToSpeed;
        state_data.speed = -10;
    }

    // THAT MUST BE BEFORE LAST IF: stop if we see white line
    if (state_data.speed > 0 && forward_right_illumination_sensor.collides() ||
        forward_left_illumination_sensor.collides())
    {
        state = State::Stop;
    }
    if (state_data.speed < 0 && backward_left_illumination_sensor.collides())
    {
        state = State::Stop;
    }
    // CHANGE IT: && false is for debug *ONLY*
    if (digitalRead(red_button_pin))
    {
        Serial.println("RED BUTTON PRESSED");
        state = State::RedButtonStopped;
        prev_state = State::RedButtonStopped;
    }
    if (prev_state == State::RedButtonStopped)
        state = State::RedButtonStopped;

#if 0 // for debugging
    if (forward_left_ultrasonic.read() < 20) {
      state = State::AccelToSpeed;
      state_data.speed = 15;
    }
    else {
      state = State::DeccelToSpeed;
      // prev_state = state;
      state_data.speed = 0;
    }
#endif
}

void apply_movement()
{
    if (state == State::AccelToSpeed)
    {
        Serial.println("ACCEL");

        if (state_data.speed >= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(motors.get_unite_speed() + 3);
        else if (state_data.speed <= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(motors.get_unite_speed() - 3);
    }
    else if (state == State::DeccelToSpeed)
    {
        Serial.println("DECCEL");

        if (state_data.speed >= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(max(0, motors.get_unite_speed() - 3));
        else if (state_data.speed <= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(min(0, motors.get_unite_speed() + 3));
    }
    else if (state == State::Stop || state == State::RedButtonStopped)
    {
        motors.move(0);
    }
}

void debug_print()
{
    Serial.print("TS: ");
    Serial.println(state_data.speed);
    Serial.print("CS: ");
    Serial.println(motors.get_unite_speed());
    Serial.println("---------------------------");
    Serial.print("FL: ");
    Serial.println(forward_left_ultrasonic.read());
    Serial.print("STATE: ");
    Serial.println(static_cast<int>(state));
    Serial.print("PREV STATE: ");
    Serial.println(static_cast<int>(state));
}