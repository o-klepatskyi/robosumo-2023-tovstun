#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"


static constexpr int START_DELAY = 1000;
static constexpr int LOOP_DELAY = 30;


void setup()
{
    Serial.begin(9600);
    infrared.init();
    motors.attach();
    motors.write_for(90, 2000);
    pinMode(red_button_pin, INPUT);
    front_left_illumination_sensor.BLACK_THRESHOLD = 300;
    front_right_illumination_sensor.BLACK_THRESHOLD = 320;
    // TODO: add back sensor
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
