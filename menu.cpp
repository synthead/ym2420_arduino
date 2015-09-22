#include "menu.h"
#include "hd44780.h"
#include "mcp23s17.h"
#include "controls.h"
#include "patch_storage.h"
#include "midi.h"
#include "keys.h"
#include "ym2420.h"
#include <Arduino.h>

#define MENU_MATRIX_COLUMN 7

#define INPUTS_ENCODER_CCW  0b00000001
#define INPUTS_ENCODER_CW   0b00000010
#define INPUTS_ENCODER_DOWN 0b00000100
#define INPUTS_SAVE         0b00001000
#define INPUTS_MIDI         0b00010000
#define INPUTS_MANUAL       0b00100000
#define INPUTS_BACK         0b01000000
#define INPUTS_PANIC        0b10000000

#define FIRST_KEY_ROW 5

#define TEMP_MESSAGE_EXPIRATION 3000

namespace Menu {
  unsigned long message_expires_ms = 0;
  uint8_t active_menu = 0;

  uint8_t read_encoder = false;
  uint8_t last_inputs = 0;

  void startup_message() {
    HD44780::print_all("Synthead YM2420", "SW version 0.1");
    set_expiration();
  }

  void set_expiration() {
    message_expires_ms = millis() + TEMP_MESSAGE_EXPIRATION;
  }

  void check_expired_message() {
    if (message_expires_ms && message_expires_ms < millis()) {
      message_expires_ms = 0;
      PatchStorage::print_patch();
    }
  }

  uint8_t new_inputs() {
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

  void user_write_patch(uint32_t id) {
    char user_input = NULL;
    uint8_t keys[6];
    uint8_t last_keys[6] = {0};
    uint8_t cursor_position = 0;
    bool shift = false;
    bool last_shift = false;
    bool shift_used = false;
    char name[PATCH_NAME_LENGTH + 1];

    strcpy(name, PatchStorage::current_name);

    if (id == FIND_NEXT_ID) {
      HD44780::print_all("Save new patch:", name);
    } else {
      char line_1[17];
      sprintf(line_1, "Update %d:", id);
      HD44780::print_all(line_1, name);
    }

    HD44780::position(0, 1);
    HD44780::cursor(true);

    while (true) {
      for (uint8_t column = 0; column < 6; column++) {
        keys[column] = MCP23S17::scan_matrix(column);
        uint8_t bitmask = ~(keys[column] & last_keys[column]);

        if (column == 5) {
          shift = keys[column] & 0b01000000;
          bitmask &= 0b10111111;

          if (last_shift && ! shift) {
            if (! shift_used) {
              user_input = ' ';
            }

            shift_used = false;
          }

          last_shift = shift;
        }

        last_keys[column] = keys[column];
        keys[column] &= bitmask;

        for (uint8_t row = 0; row < 7; row++) {
          if ((keys[column] >> row) & 1) {
            uint8_t key = row * 6 + column - FIRST_KEY_ROW;

            if (key <= 25) {
              if (shift) {
                user_input = 'A' + key;
                shift_used = true;
              } else {
                user_input = 'a' + key;
              }
            } else if (key <= 34) {
              user_input = '1' + key - 26;
            } else {
              user_input = '0';
            }
          }
        }
      }

      if (user_input != NULL) {
        char character[2];
        sprintf(character, "%c", user_input);
        HD44780::position_print(cursor_position, 1, character);

        name[cursor_position] = user_input;
        user_input = NULL;

        if (cursor_position == PATCH_NAME_LENGTH - 1) {
          HD44780::position(PATCH_NAME_LENGTH - 1, 1);
        }
        if (cursor_position < PATCH_NAME_LENGTH - 1) {
          cursor_position++;
        }
      }

      uint8_t inputs = new_inputs();

      if (inputs & INPUTS_ENCODER_CCW && cursor_position != 0) {
        cursor_position--;
        HD44780::position(cursor_position, 1);
      } else if (inputs & INPUTS_ENCODER_CW &&
                 cursor_position != PATCH_NAME_LENGTH - 1) {
        cursor_position++;
        HD44780::position(cursor_position, 1);
      }

      if (inputs & INPUTS_SAVE) {
        PatchStorage::write(id, name);
        break;
      }

      if (inputs & INPUTS_BACK) {
        PatchStorage::print_patch();
        break;
      }
    }
  }

  void midi_settings() {
    char line_2[17];
    sprintf(line_2, "Channel: %d", MIDI::channel + 1);
    HD44780::print_all("MIDI settings:", line_2);
  }

  void scan_inputs() {
    uint8_t inputs = new_inputs();

    if (inputs & INPUTS_PANIC) {
      MIDI::setup();
      Keys::panic();
      YM2420::panic();
    }

    switch (active_menu) {
      case INPUTS_MIDI:
        if (inputs & INPUTS_ENCODER_CW) {
          MIDI::next_channel();
          midi_settings();
        } else if (inputs & INPUTS_ENCODER_CCW) {
          MIDI::previous_channel();
          midi_settings();
        }

        if (inputs & INPUTS_BACK) {
          active_menu = 0;
          PatchStorage::print_patch();
        }

        break;
      case 0:
        if (inputs & INPUTS_SAVE) {
          user_write_patch(FIND_NEXT_ID);
        }

        if (inputs & INPUTS_ENCODER_DOWN) {
          user_write_patch(PatchStorage::current_id);
        }

        if (inputs & INPUTS_MIDI) {
          active_menu = INPUTS_MIDI;
          midi_settings();
        }

        if (inputs & INPUTS_MANUAL) {
          Controls::set_current_values();
          PatchStorage::new_patch();
        }

        if (inputs & INPUTS_ENCODER_CW) {
          PatchStorage::read_next();
        } else if (inputs & INPUTS_ENCODER_CCW) {
          PatchStorage::read_previous();
        }

        break;
    }
  }
}
