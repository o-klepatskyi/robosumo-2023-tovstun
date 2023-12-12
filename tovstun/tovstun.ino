#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"
// #include "display.hpp"
// #include "debug.hpp"

static constexpr int START_DELAY = 5000;
static constexpr int LOOP_DELAY = 100;

void setup()
{
    Serial.begin(9600);
    motors.attach();
    motors.write_for(90, 2000);
    pinMode(red_button_pin, INPUT);
    Serial.println("Hello robot!");
}

bool start = false;

void loop()
{
    if (!start)
    {
        if (digitalRead(red_button_pin))
        {
            start = true;
            Serial.println("Starting 5 sec delay");
            delay(START_DELAY);
            return;
        }
        delay(LOOP_DELAY);
        return;
    }

    // either start moving or print IR sensors value
    // debug_display_print_illumination();
    on_loop();
    delay(LOOP_DELAY);
}
