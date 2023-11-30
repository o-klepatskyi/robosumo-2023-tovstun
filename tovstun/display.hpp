#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


const int display_i2c_address = 0x27;

struct Display {
public:
  Display(int address, int col_count, int row_count)
    : lcd{ address, col_count, row_count }, max_row_count{ row_count } {
    lcd.init();
  }

  /**
   * Prints multiple rows to a display
   */
  void print(const char* const* data, int row_count) {
    lcd.clear();

    for (int row_index = 0; row_index < min(row_count, max_row_count); ++row_index) {
      lcd.setCursor(0, row_index);
      lcd.print(data[row_index]);
    }
  }

  /**
   * Prints a single row to a display
   */
  void print(const char* data) {
    print(&data, 1);
  }

private:
  const int max_row_count;
  LiquidCrystal_I2C lcd;
};

extern Display display;
