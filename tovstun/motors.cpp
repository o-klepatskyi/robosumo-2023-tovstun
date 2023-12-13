#include "motors.hpp"

Motors motors;

void Motors::attach()
{
    l_motor.attach(left_motor_pin);
    r_motor.attach(right_motor_pin);
    write_for(90, 2000);
}

void Motors::prepare_forward(bool left, bool right)
{
    // do we need to prepare them at all?
    left = left && l_motor.prev_speed() < 90;
    right = right && r_motor.prev_speed() < 90;
    if (!left && !right)
        return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration)
    {
        if (left)  l_motor.write(90);
        if (right) r_motor.write(90);
    }
}

void Motors::prepare_backward(bool left, bool right)
{
    // do we need to prepare them at all?
    left = left && l_motor.prev_speed() >= 90;
    right = right && r_motor.prev_speed() >= 90;
    if (!left && !right)
        return;
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration)
    {
        if (left)  l_motor.write(87);
        if (right) r_motor.write(87);
    }
    prevMillis = millis();
    while (millis() - prevMillis <= prepare_write_duration)
    {
        if (left)  l_motor.write(90);
        if (right) r_motor.write(90);
    }
}

void Motors::move(int speed)
{
    int actual_speed = speed + 90;
    if (speed == 0)
    {
        write_for(90, 20);
    }
    else if (speed > 0)
    {
        prepare_forward();
        write_for(actual_speed, 40);
    }
    else if (speed < 0)
    {
        prepare_backward();
        write_for(actual_speed, 40);
    }
}

void Motors::prepare_left_forward_right_backward()
{
    prepare_forward(true, false);
    prepare_backward(false, true);
}

void Motors::prepare_left_backward_right_forward()
{
    prepare_forward(false, true);
    prepare_backward(true, false);
}

void Motors::prepare_left()
{
    // prepare only right motor
    prepare_backward(false, true);
}

void Motors::prepare_right()
{
    // prepare only left motor
    prepare_backward(true, false);
}

void Motors::write_for(int speed, unsigned long miliseconds)
{
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds)
    {
        l_motor.write(speed);
        r_motor.write(speed);
    }
}

void Motors::write_for_l(int speed, unsigned long miliseconds)
{
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds)
    {
        l_motor.write(speed);
    }
}

void Motors::write_for_r(int speed, unsigned long miliseconds)
{
    unsigned long prevMillis = millis();
    while (millis() - prevMillis <= miliseconds)
    {
        r_motor.write(speed);
    }
}

void Motors::rotate_left(int speed)
{
    prepare_left();
    write_for_r(speed, 50);
}

void Motors::rotate_right(int speed)
{
    prepare_right();
    write_for_l(speed, 50);
}

void Motors::rotate_left_still(int forward, int backward, unsigned long long duration)
{
    prepare_left_backward_right_forward();
    unsigned long prevMillis = millis();
    while (millis() - prevMillis < duration)
    {
        l_motor.write(-backward + 90);
        r_motor.write(forward + 90);
    }
}

void Motors::rotate_right_still(int forward, int backward, unsigned long long duration)
{
    prepare_left_forward_right_backward();
    unsigned long prevMillis = millis();

    while (millis() - prevMillis < duration)
    {
        l_motor.write(forward + 90);
        r_motor.write(-backward + 90);
    }
}

void Motors::rotate_left_90_degrees()
{
    rotate_left_still(DEFAULT_ROTATION_SPEED, 2  * DEFAULT_ROTATION_SPEED, ROT_90_DEG_DURATION);
    stop();
}

void Motors::rotate_right_90_degrees()
{
    rotate_right_still(DEFAULT_ROTATION_SPEED, 2  * DEFAULT_ROTATION_SPEED, ROT_90_DEG_DURATION);
    stop();
}