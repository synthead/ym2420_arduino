#include "menu.h"
#include "hd44780.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define MENU_MATRIX_COLUMN 7

#define INPUTS_ENCODER_CCW  0b00000001
#define INPUTS_ENCODER_CW   0b00000010
#define INPUTS_ENCODER_DOWN 0b00000100

#define TEMP_MESSAGE_EXPIRATION 3000

namespace Menu {
  temporary_message_t temporary_message;

  uint8_t read_encoder = false;
  uint8_t last_inputs = 0;

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

  uint8_t scan_inputs() {
    uint8_t inputs = MCP23S17::scan_matrix(MENU_MATRIX_COLUMN);
    uint8_t new_inputs = inputs & ~(inputs & last_inputs) & 0b11111100;

    uint8_t encoder = inputs & 0b11;
    if (read_encoder && encoder == 0b00) {
      switch (last_inputs & 0b11) {
        case 0b01:
          new_inputs |= INPUTS_ENCODER_CCW;
          break;
        case 0b10:
          new_inputs |= INPUTS_ENCODER_CW;
          break;
      }

      read_encoder = false;
    } else if (! read_encoder && encoder == 0b11) {
      read_encoder = true;
    }

    last_inputs = inputs;
    return new_inputs;
  }
}