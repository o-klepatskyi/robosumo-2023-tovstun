#pragma once

#include <Arduino.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <VL53L0X.h>

// ultrasonic sensors
const int forward_left_ultrasonic_trig_pin = 4;
const int forward_left_ultrasonic_echo_pin = 5;
extern Ultrasonic front_left_ultrasonic;

const int forward_right_ultrasonic_trig_pin = 3;
const int forward_right_ultrasonic_echo_pin = 2;
extern Ultrasonic front_right_ultrasonic;

const int left_ultrasonic_trig_pin = 8;
const int left_ultrasonic_echo_pin = 11;
extern Ultrasonic left_ultrasonic;

const int right_ultrasonic_trig_pin = 6;
const int right_ultrasonic_echo_pin = 7;
extern Ultrasonic right_ultrasonic;

// illumination sensors (ANALOG)
const int forward_left_illumination_pin = A0;
const int forward_right_illumination_pin = A1;
const int backward_illumination_pin = A2;

//infrared sensor
struct Infrared_sensor {
  VL53L0X sensor;
  void init() 
  {
    Wire.begin();
    sensor.setTimeout(50);
    sensor.init();
    sensor.setMeasurementTimingBudget(20000);
  }
  int read() { return sensor.readRangeSingleMillimeters(); }
};
extern Infrared_sensor infrared;


const int red_button_pin = 13;

struct Illumination_sensor
{
    // TODO: recheck this value
    int BLACK_THRESHOLD = 1000;
    int pin;

    Illumination_sensor(int pin)
        : pin{pin}
    {
        pinMode(pin, INPUT);
    }

    /**
     * Is there an obstacle?
     */
    bool collides() { return read() >= BLACK_THRESHOLD; }

    int read() { return analogRead(pin); }
};

extern Illumination_sensor front_left_illumination_sensor;
extern Illumination_sensor front_right_illumination_sensor;
extern Illumination_sensor back_illumination_sensor;


static constexpr int FRONT_DETECTION_THRESHOLD_CLOSE = 10;
static constexpr int FRONT_DETECTION_THRESHOLD = 50;
static constexpr int FRONT_RIGHT_DETECTION_THRESHOLD = 45;
static constexpr int BACK_DETECTION_THRESHOLD = 500; // !!!milimeters!!!
static constexpr int SIDE_DETECTION_THRESHOLD = 50;

struct SensorsData {
  bool is_fl_obstacle; // detected line on front left sensor
  bool is_fr_obstacle; // detected line on front right sensor
  bool is_back_obstacle; // detected line on back sensor

  int fl_us_value; // front left ultrasonic value
  int fr_us_value; // front right ultrasonic value
  int l_us_value; // left ultrasonic value
  int r_us_value; // right ultrasonic value
  int b_ir_value; // back infrared value

  static SensorsData read() {
    auto fli = front_left_illumination_sensor.collides();
    auto fri = front_right_illumination_sensor.collides();
    auto bi = back_illumination_sensor.collides();
    auto flu = front_left_ultrasonic.read();
    auto fru = front_right_ultrasonic.read();
    auto lu = left_ultrasonic.read();
    auto ru = right_ultrasonic.read();
    auto ir = infrared.read();
    return SensorsData {
      fli, fri, bi,
      flu, fru, lu, ru, ir
    };
  }

  bool fl_detects_enemy() const noexcept
  {
    return fl_us_value <= FRONT_DETECTION_THRESHOLD && fl_us_value != 0;
  }

  bool fr_detects_enemy() const noexcept
  {
    return fr_us_value <= FRONT_RIGHT_DETECTION_THRESHOLD && fr_us_value != 0;
  }

  bool left_detects_enemy() const noexcept
  {
    return l_us_value <= SIDE_DETECTION_THRESHOLD && l_us_value != 0;
  }

  bool right_detects_enemy() const noexcept
  {
    return r_us_value <= SIDE_DETECTION_THRESHOLD && r_us_value != 0;
  }

  bool front_detects_enemy() const noexcept
  {
    return fl_detects_enemy() || fr_detects_enemy();
  }

  bool back_detects_enemy() const noexcept
  {
    return b_ir_value <= BACK_DETECTION_THRESHOLD && b_ir_value != 0;
  }
  bool no_enemy_detected() const noexcept
  {
    return !front_detects_enemy() && !left_detects_enemy() && !right_detects_enemy();
  }

  bool edge_detected() const noexcept
  {
    return is_fl_obstacle || is_fr_obstacle || is_back_obstacle;
  }

  bool enemy_close_front() const noexcept
  {
    return (fl_us_value <= FRONT_DETECTION_THRESHOLD_CLOSE && fl_us_value != 0)
      || (fr_us_value <= FRONT_DETECTION_THRESHOLD_CLOSE && fr_us_value != 0);
  }
};
