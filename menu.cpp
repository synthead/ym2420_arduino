#include "menu.h"
#include "hd44780.h"
#include <Arduino.h>

#define TEMP_MESSAGE_EXPIRATION 3000

namespace Menu {
  temporary_message_t temporary_message;

  void startup_message() {
    HD44780::print_position(0, 0, "Synthead YM2420 ");
    HD44780::print_position(0, 1, "SW version 0.1  ");

    set_temporary_message();
  }

  void set_temporary_message() {
    temporary_message.displayed_at = millis();
    temporary_message.expired = false;
  }

  void check_expired_message() {
    if (! temporary_message.expired &&
        temporary_message.displayed_at + TEMP_MESSAGE_EXPIRATION < millis()) {
      temporary_message.expired = true;

      HD44780::print_position(0, 0, "000: I made it! ");
      HD44780::print_position(0, 1, "                ");
    }
  }
}
