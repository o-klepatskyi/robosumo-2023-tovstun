#pragma once

enum class State {
  Default = 0, // State when the round hasn't yet started, but sensors can be read
  Stop,
  StartAccel,
  AccelToSpeed,
  AccelToSpeedEnemyOnTheLeft,
  AccelToSpeedEnemyOnTheRight,
  DecelToSpeed,
  HoldSpeed,
  StartRotateLeftStill,
  StartRotateRightStill,
  RotatingLeftStill,
  RotatingRightStill,
  // RotateLeftBack,
  // RotateRightBack,
  RotateLeft90Degrees,
  RotateRight90Degrees,
  RedButtonStopped,
};

struct StateData {
  int speed = 0;
  int duration = 1000;
};

static const char* state_to_string(State state) noexcept
{
    switch (state)
    {
    case State::Default:
        return "Default";
    case State::Stop:
        return "Stop";
    case State::StartAccel:
        return "StartAccel";
    case State::AccelToSpeed:
        return "AccelToSpeed";
    case State::DecelToSpeed:
        return "DecelToSpeed";
    case State::HoldSpeed:
        return "HoldSpeed";
    case State::StartRotateLeftStill:
        return "StartRotateLeftStill";
    case State::StartRotateRightStill:
        return "StartRotateRightStill";
    case State::RotatingLeftStill:
        return "RotatingLeftStill";
    case State::RotatingRightStill:
        return "RotatingRightStill";
    // case State::RotateLeftBack:
    //     return "RotateLeftBack";
    // case State::RotateRightBack:
    //     return "RotateRightBack";
    case State::RotateLeft90Degrees:
        return "RotateLeft90Degrees";
    case State::RotateRight90Degrees:
        return "RotateRight90Degrees";
    case State::RedButtonStopped:
        return "RedButtonStopped";
    default:
        return "Unknown";
    }
}
