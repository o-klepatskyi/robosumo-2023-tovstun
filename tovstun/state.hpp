#pragma once


enum class State {
  Default = 0, // State when the round hasn't yet started, but sensors can be read
  Stopped,
  Stop,
  AccelToSpeed,
  DeccelToSpeed,
  HoldSpeed,
  RotateStillLeft,
  RotateStillRight,
  RotateLeftBack,
  RotateRightBack,
  RedButtonStopped,
};

struct StateData {
  int speed = 0;
};
