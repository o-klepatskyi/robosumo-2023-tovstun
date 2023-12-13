#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"
#include <Arduino.h> // Serial.println

unsigned long state_duration = 0;
StateData state_data {};
State state = State::Default;
State next_state = State::Default;
State prev_state = State::Default;

static bool moving_forwards() noexcept
{
    return (state == State::AccelToSpeed || state == State::DecelToSpeed || state == State::HoldSpeed) && state_data.speed > 0;
}

static bool moving_backwards() noexcept
{
    return ((state == State::AccelToSpeed || state == State::DecelToSpeed || state == State::HoldSpeed) && state_data.speed < 0);
            // || state == State::RotateLeftBack || state == State::RotateRightBack;
}

static bool stopped() noexcept
{
    return state == State::Stop && motors.get_unite_speed() == 0;
}

State checkButton()
{
    // this is terminal state
    if (state == State::RedButtonStopped)
        return State::RedButtonStopped;

    if (digitalRead(red_button_pin))
        return State::RedButtonStopped;
    
    return state;
}

// only use this for testing
State rotate_test(const SensorsData& sensors)
{
    // if (State::RedButtonStopped == checkButton())
    //     return State::RedButtonStopped;
    
    if (state == State::StartRotateLeftStill)
        return State::RotatingLeftStill;

    if (state == State::StartRotateRightStill)
        return State::RotatingRightStill;
    
    if (state == State::Default)
    {
        state_data.duration = 1000;
        state_data.speed = DEFAULT_ROTATION_SPEED;
        return State::StartRotateLeftStill;
    }

    if (state == State::RotatingLeftStill || state == State::RotatingRightStill)
    {
        if (state_duration >= state_data.duration)
        {
            state_data.duration = 1000;
            return State::Stop;
        }
        return state;
    }

    if (state == State::Stop)
    {
        if (state_duration >= state_data.duration)
        {
            return State::Default;
        }
        return state;
    }

    return state;
}

// only use this for testing
State rotate_loop(const SensorsData& sensors)
{
    static int rotation = 0;
    if (State::RedButtonStopped == checkButton())
        return State::RedButtonStopped;
    
    if (state == State::StartRotateLeftStill)
        return State::RotatingLeftStill;

    if (state == State::StartRotateRightStill)
        return State::RotatingRightStill;

    if (state == State::Default)
    {
        state_data.duration = 1000;
        state_data.speed = DEFAULT_SPEED;
        return State::AccelToSpeed;
    }

    if (state == State::AccelToSpeed)
    {
        if (state_duration >= state_data.duration)
        {
            state_data.duration = 1000; // change this duration
            state_data.speed = DEFAULT_ROTATION_SPEED;
            if (rotation == 0)
                return State::StartRotateLeftStill;
            else
                return State::StartRotateRightStill;
        }
        return state;
    }

    if (state == State::RotatingLeftStill)
    {
        if (state_duration >= state_data.duration)
        {
            rotation = 1;
            state_duration = 1000;
            return State::Stop;
        }
        return state;
    }

    if (state == State::RotatingRightStill)
    {
        if (state_duration >= state_data.duration)
        {
            rotation = 0;
            state_data.duration = 1000;
            return State::Stop;
        }
        return state;
    }

    if (state == State::Stop)
    {
        if (state_duration >= state_data.duration)
        {
            state_data.duration = 2000;
            state_data.speed = DEFAULT_SPEED;
            return State::AccelToSpeed;
        }
        return state;
    }

    return state;
}

// defines state transitions if we detect edge line
// TODO: we need to improve the way robot moves away from the edge
static State move_from_edge(const SensorsData& sensors)
{
    // TODO: fix rotations
    #if 0 
    // if we only detect line behind, stop and accelerate forward
    if (sensors.is_back_obstacle && !sensors.is_fl_obstacle && !sensors.is_fr_obstacle)
    {
        if (moving_forwards())
        {
            return state; // OK, continue moving into center
        }
        else if (stopped())
        {
            state_data.speed = DEFAULT_SPEED;
            return State::AccelToSpeed;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // If we only detect line by the left sensor, move left back
    // this way we will face the center
    if (!sensors.is_back_obstacle && sensors.is_fl_obstacle && !sensors.is_fr_obstacle)
    {
        if (state == State::RotateLeftBack) // or rotate right still
        {
            return state; // OK, continue rotating until we do not see the edge
        }
        else if (stopped())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateLeftBack;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // If we only detect line by the right sensor, move right back
    // this way we will face the center
    if (!sensors.is_back_obstacle && !sensors.is_fl_obstacle && sensors.is_fr_obstacle)
    {
        if (state == State::RotateRightBack) // or rotate left still
        {
            return state; // OK, continue rotating until we do not see the edge
        }
        else if (stopped())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateRightBack;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // If we detect line by both front sensors, stop and start moving backwards
    // TODO: we need to make sure we won't start moving forward after that (maybe move back and rotate?)
    if (!sensors.is_back_obstacle && sensors.is_fl_obstacle && sensors.is_fr_obstacle)
    {
        if (moving_backwards())
        {
            return state; // OK, continue moving back into center
        }
        else if (stopped())
        {
            state = State::AccelToSpeed;
            state_data.speed = -DEFAULT_SPEED;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // If we detect line by both back and left sensors, rotate right until only back sensor detects line
    if (sensors.is_back_obstacle && sensors.is_fl_obstacle && !sensors.is_fr_obstacle)
    {
        if (state == State::RotateRightStill) // or just move forward
        {
            return state;
        }
        else if (stopped())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateRightStill;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // If we detect line by both back and right sensors, rotate left until only back sensor detects line
    if (sensors.is_back_obstacle && !sensors.is_fl_obstacle && sensors.is_fr_obstacle)
    {
        if (state == State::RotateLeftStill) // or just move forward
        {
            return state;
        }
        else if (stopped())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateLeftStill;
        }

        state_data.speed = 0;
        return State::Stop;
    }

    // if we are here, either we detect nothing or detect line by all sensors
    // we will stop just in case
    state_data.speed = 0;
    return State::Stop;
    #endif
    return state;
}

// We do not assign state value directly
// We return the next state value
State state_transition(const SensorsData& sensors)
{
    if (State::RedButtonStopped == checkButton())
        return State::RedButtonStopped;

    // Firstly check the edge
    if (sensors.edge_detected())
        return move_from_edge(sensors);

    if (state == State::Default)
    {
        if (sensors.front_detects_enemy())
        {
            state_data.speed = DEFAULT_SPEED;
            return State::AccelToSpeed;
        }
        else if (sensors.left_detects_enemy())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateLeft90Degrees;
        }
        else if (sensors.right_detects_enemy())
        {
            state_data.speed = DEFAULT_ROTATION_SPEED;
            return State::RotateRight90Degrees;
        }

        state_data.speed = -DEFAULT_SPEED;
        return State::AccelToSpeed;
    }

    // if there is something in the front: accel + positive speed
    // TODO: maybe we need to align ourselves with the enemy so both sensors see him???
    if (sensors.front_detects_enemy())
    {
        state_data.speed = DEFAULT_SPEED;
        return State::AccelToSpeed;
    }
     if (sensors.left_detects_enemy())
     {
        state_data.speed = DEFAULT_ROTATION_SPEED;
        return State::RotateLeft90Degrees;
     }
     if (sensors.right_detects_enemy())
     {
        state_data.speed = DEFAULT_ROTATION_SPEED;
        return State::RotateRight90Degrees;
     }
     if (sensors.back_detects_enemy())
     {
        state_data.speed = -DEFAULT_SPEED;
        return State::AccelToSpeed;
     }

    // if there is nothing at front or sideways or back we will rotate righ while we will not see _something_ with front
    // sensor
    // TODO: incorrect behaviour, enemy must be behind us
    if (sensors.no_enemy_detected())
    {
        state_data.speed = DEFAULT_ROTATION_SPEED;
        return State::StartRotateRightStill;
    }

#if 0 // for debugging
    if (front_left_ultrasonic.read() < 20) {
      state = State::AccelToSpeed;
      state_data.speed = 15;
    }
    else {
      state = State::DecelToSpeed;
      // prev_state = state;
      state_data.speed = 0;
    }
#endif
    return state; // by default the same state is remained
}

void apply_movement()
{
    if (state == State::AccelToSpeed)
    {
        if (state_data.speed >= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(min(state_data.speed, motors.get_unite_speed() + DEFAULT_ACCELERATION));
        else if (state_data.speed <= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(max(state_data.speed, motors.get_unite_speed() - DEFAULT_ACCELERATION));
    }
    else if (state == State::DecelToSpeed)
    {
        if (state_data.speed >= 0 && motors.get_unite_speed() > state_data.speed)
            motors.move(max(state_data.speed, motors.get_unite_speed() - DEFAULT_ACCELERATION));
        else if (state_data.speed <= 0 && motors.get_unite_speed() < state_data.speed)
            motors.move(min(state_data.speed, motors.get_unite_speed() + DEFAULT_ACCELERATION));
    }
    else if (state == State::StartRotateLeftStill)
    {
        motors.rotate_left_still(state_data.speed, 2 * state_data.speed);
    }
    else if (state == State::StartRotateRightStill)
    {
        motors.rotate_right_still(state_data.speed, 2 * state_data.speed);
    }
    // else if (state == State::RotateLeftBack)
    // {
        // if (motors.get_unite_speed() == 0)
        // {
        //     motors.rotate_left(state_data.speed);
        // }
    // }
    // else if (state == State::RotateRightBack)
    // {
        // if (motors.get_unite_speed() == 0)
        // {
        //     motors.rotate_right(state_data.speed);
        // }
    // }
    else if (state == State::RotateLeft90Degrees)
    {
       motors.rotate_left_90_degrees();
    }
    else if (state == State::RotateRight90Degrees)
    {
       motors.rotate_right_90_degrees();
    }
    else if (state == State::Stop || state == State::RedButtonStopped || state == State::Default)
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
