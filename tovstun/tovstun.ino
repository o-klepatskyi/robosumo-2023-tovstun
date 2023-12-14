#include "logic.hpp"
#include "motors.hpp"
#include "sensors.hpp"
#include "state.hpp"

static constexpr int START_DELAY = 5000;
static constexpr int LOOP_DELAY = 1;

static constexpr int ROUND_TIME = 130 * 1000;
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
    front_left_illumination_sensor.BLACK_THRESHOLD = 310;
    front_right_illumination_sensor.BLACK_THRESHOLD = 450;
    back_illumination_sensor.BLACK_THRESHOLD = 350;
    // TODO: add back sensor
    Serial.println("\n\nHello robot!");
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
        motors.stop();
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
    // print_sensors(sensors);
    // print_ir_sensors();

    // Serial.print("ir: ");
    // Serial.println(sensors.b_ir_value);

    State newState = state_transition(sensors);
    // State newState = empty(sensors);

    if (newState != state)
    {
        state_duration = 0;
        state = newState;
    }

    // print_state();

    apply_movement();

    delay(LOOP_DELAY);
}
