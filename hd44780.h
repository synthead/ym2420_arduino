#ifndef LCD_H
#define LCD_H

#include <stdint.h>  // For uint8_t.

void hd44780_print(const char*);
void hd44780_print_position(uint8_t, uint8_t, const char*);
void hd44780_setup();

#endif
