#pragma once
#include "state.hpp"
#include "sensors.hpp"

extern unsigned long state_duration; // For how long current state lasts
extern StateData state_data;
extern State state;
extern State next_state;
extern State prev_state;

// decide what state in next depending on the sensors values
State state_transition(const SensorsData& sensors);
State rotate_test(const SensorsData& sensors);
State rotate_loop(const SensorsData& sensors);

// sends signals to motors depending on the current state
void apply_movement();

void debug_print();