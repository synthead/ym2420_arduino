#ifndef LCD_H
#define LCD_H

#include <stdint.h>  // For uint8_t.

namespace HD44780 {
  void print(const char*);
  void print_position(uint8_t, uint8_t, const char*);
  void setup();
}

#endif
