#pragma once
#include <Arduino.h>
#include <Servo.h>

// motors
const int left_motor_pin = 9;
const int right_motor_pin = 10;

// servo for the flag
const int flag_servo_pin = 11;

// Assuming these constants are defined based on our robot's design
// TODO: define the right constants
const float wheel_diameter = 0.065; // in meters
const float wheelbase = 0.16;       // in meters
const float wheel_circumference = PI * wheel_diameter;
const float rotation_circumference = PI * wheelbase;
const float rotation_distance = rotation_circumference / 4; // For 90 degrees
const float rotation_speed = 0.2;                           // Speed of wheel rotation in meters per second

// const int rotation_90_degree_forward = 95;
// const int rotation_90_degree_backward = 80;
const unsigned long ROT_90_DEG_DURATION = 1500; // TODO:

// TODO: review this constants
static constexpr int DEFAULT_ACCELERATION = 1; // amount of speed we add each loop
static constexpr int DEFAULT_SPEED = 6;
static constexpr int DEFAULT_START_SPEED = 3;
static constexpr int DEFAULT_ROTATION_SPEED = 6; // for negative speed it will be twice as big

struct Flag
{
    Servo flag;

    static constexpr int RAISED = 120;
    static constexpr int LOWERED = 50;

    Flag() { flag.attach(flag_servo_pin); }

    void raise() { flag.write(RAISED); }

    void lower() { flag.write(LOWERED); }
};

extern Flag flag;

class Motor
{
    Servo servo;
    int motor_speed = 90;
public:
    void attach(int pin) { servo.attach(pin); }
    void write(int speed) { servo.write(speed); motor_speed = speed; }
    int prev_speed() const { return motor_speed; }
};

struct Motors
{
    static constexpr unsigned long prepare_write_duration = 100;
    Motor l_motor, r_motor;

    void attach();

    void prepare_forward(bool left = true, bool right = true);

    void prepare_backward(bool left = true, bool right = true);

    void move(int speed);

    void prepare_left_forward_right_backward();

    void prepare_left_backward_right_forward();

    void prepare_left();

    void prepare_right();

    void write_for(int speed, unsigned long miliseconds);

    void write_for_l(int speed, unsigned long miliseconds);

    void write_for_r(int speed, unsigned long miliseconds);

    void rotate_left(int speed);

    void rotate_right(int speed);

    void rotate_left_still(int forward, int backward, unsigned long long duration = 50);

    void rotate_right_still(int forward, int backward, unsigned long long duration = 50);

    void rotate_left_90_degrees();

    void rotate_right_90_degrees();

    void stop() { move(0); }

    // ASSUME: speed for motors is equal
    int get_unite_speed() { return l_motor.prev_speed() - 90; }
};

extern Motors motors;
