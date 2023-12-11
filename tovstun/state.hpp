#pragma once


enum class State {
  Default = 0, // State when the round hasn't yet started, but sensors can be read
  Stop,
  AccelToSpeed,
  DeccelToSpeed,
  HoldSpeed,
  RotateLeftStill,
  RotateRightStill,
  RotateLeftBack,
  RotateRightBack,
  RedButtonStopped,
};

struct StateData {
  int speed = 0;
};

static const char* state_to_string(State state) noexcept
{
    switch (state)
    {
    case State::Default:
        return "Default";
    case State::Stop:
        return "Stop";
    case State::AccelToSpeed:
        return "AccelToSpeed";
    case State::DeccelToSpeed:
        return "DeccelToSpeed";
    case State::HoldSpeed:
        return "HoldSpeed";
    case State::RotateLeftStill:
        return "RotateLeftStill";
    case State::RotateRightStill:
        return "RotateRightStill";
    case State::RotateLeftBack:
        return "RotateLeftBack";
    case State::RotateRightBack:
        return "RotateRightBack";
    case State::RedButtonStopped:
        return "RedButtonStopped";
    default:
        return "Unknown";
    }
}
