#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"
// #include "display.hpp"
// #include "debug.hpp"

void setup()
{
    Serial.begin(9600);
    motors.attach();
    motors.write_for(90, 2000);
    pinMode(red_button_pin, INPUT);
    Serial.println("Hello world!");
}

void loop()
{
    // either start moving or print IR sensors value
    // debug_display_print_illumination();
    on_loop();
    delay(500);
}
