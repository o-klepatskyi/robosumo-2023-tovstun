#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"

static constexpr int START_DELAY = 1000;
static constexpr int LOOP_DELAY = 30;

static constexpr int ROUND_TIME = 10 * 1000;
static int start_time = 0;
static bool started = false;

static unsigned long prev_time = 0;

bool is_round_finished() noexcept
{
    return millis() - start_time >= ROUND_TIME;
}

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

void loop()
{
    // wait for button press first, then start 5 sec delay
    if (!started)
    {
        if (digitalRead(red_button_pin))
        {
            started = true;
            Serial.println("Starting 5 sec delay");
            delay(START_DELAY);

            start_time = millis();
            prev_time = millis();
            return;
        }
        delay(LOOP_DELAY);
        return;
    }

    if (is_round_finished())
    {
        Serial.println("Round finished");
        delay(LOOP_DELAY);
        return;
    }

    // either start moving or print IR sensors value
    // debug_display_print_illumination();

    // main loop
    const unsigned long curr_time = millis();
    const unsigned long dt = curr_time - prev_time;
    prev_time = curr_time;
    state_duration += dt;

    prev_state = state;
    const SensorsData sensors = SensorsData::read();

    State newState = state_transition(sensors);

    if (newState != state)
    {
        state_duration = 0;
        state = newState;
    }

    Serial.print("STATE:");
    Serial.print(state_duration);
    Serial.print(" | duration=");
    Serial.print(state_data.duration);
    Serial.print(", speed=");
    Serial.println(state_data.speed);
    Serial.println(state_to_string(state));

    apply_movement();

    delay(LOOP_DELAY);
}
