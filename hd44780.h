#ifndef LCD_H
#define LCD_H

#include <stdint.h>  // For uint8_t.

struct temporary_message_t {
  unsigned long displayed_at;
  bool expired;
};

extern temporary_message_t temporary_message;

void hd44780_print(const char*);
void hd44780_print_position(uint8_t, uint8_t, const char*);
void hd44780_setup();
void set_temporary_message();
void check_expired_message();

#endif
