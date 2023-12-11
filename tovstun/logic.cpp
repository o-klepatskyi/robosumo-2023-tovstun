#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"
#include <Arduino.h> // Serial.println

static State state = State::Default;
static State next_state = State::Default;
static State prev_state = State::Default;
// TODO: check the right value for black surface
const int black_threshold = 500;

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

    Serial.print("STATE: ");
    Serial.println(state_to_string(state));

    apply_movement();

    // if (state == next_state)
    // return;

    // state = next_state;
    // state_duration = 0;

    // SensorsData sensors = SensorsData::read();
    // state_transition(sensors);
    // apply_movement();
}

static bool moving_forwards() noexcept
{
    return (state == State::AccelToSpeed || state == State::DeccelToSpeed || state == State::HoldSpeed) && state_data.speed > 0;
}

static bool moving_backwards() noexcept
{
    return (state == State::AccelToSpeed || state == State::DeccelToSpeed || state == State::HoldSpeed) && state_data.speed < 0;
}

static bool stopped() noexcept
{
    return state == State::Stop && motors.get_unite_speed() == 0;
}

void state_transition(const SensorsData& sensors)
{
    // this is terminal state
    if (prev_state == State::RedButtonStopped)
    {
        state = State::RedButtonStopped;
        return;
    }

    // TODO: make a class for red button
    if (digitalRead(red_button_pin))
    {
        state = State::RedButtonStopped;
        state_data = {};
        return;
    }

    // if we only detect line behind, stop and accelerate forward
    if (sensors.is_back_obstacle && !sensors.is_fl_obstacle && !sensors.is_fl_obstacle)
    {
        if (moving_backwards())
        {
            state == State::Stop;
            state_data.speed = 0;
        }
        else if (stopped())
        {
            state == State::AccelToSpeed;
            state_data.speed = 10;
        }

        return;
    }

    // TODO: only left sensor, only right sensor, both front sensors, left and back, right and back

    // if there is something in the front: accel + positive speed
    if ((front_left_ultrasonic.read() <= 20 && front_left_ultrasonic.read() != 0) ||
        (front_right_ultrasonic.read() <= 20 && front_right_ultrasonic.read() != 0))
    {
        state = State::AccelToSpeed;
        state_data.speed = 10;
        // if there is nothing at front or sideways, accel + negative speed
    }
    else if (front_left_ultrasonic.read() > 20 && front_right_ultrasonic.read() > 20)
    {
        state = State::AccelToSpeed;
        state_data.speed = -10;
    }

#if 0 // for debugging
    if (front_left_ultrasonic.read() < 20) {
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
        if (state_data.speed >= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(motors.get_unite_speed() + 3);
        else if (state_data.speed <= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(motors.get_unite_speed() - 3);
    }
    else if (state == State::DeccelToSpeed)
    {
        if (state_data.speed >= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(max(0, motors.get_unite_speed() - 3));
        else if (state_data.speed <= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(min(0, motors.get_unite_speed() + 3));
    }
    else if (state == State::RotateLeftStill)
    {
        motors.rotate_left_still(10);
    }
    else if (state == State::RotateRightStill)
    {
        motors.rotate_right_still(10);
    }
    else if (state == State::Stop || state == State::RedButtonStopped)
    {
        motors.stop();
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
    Serial.println(front_left_ultrasonic.read());
    Serial.print("STATE: ");
    Serial.println(static_cast<int>(state));
    Serial.print("PREV STATE: ");
    Serial.println(static_cast<int>(state));
}