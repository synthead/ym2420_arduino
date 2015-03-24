#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

namespace HD44780 {
  void cursor(bool);
  void print(const char*);
  void position(uint8_t, uint8_t);
  void position_print(uint8_t, uint8_t, const char*);
  void print_all(const char*, const char*);
  void setup();
}

#endif
