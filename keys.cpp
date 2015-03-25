#include "keys.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "hd44780.h"
#include "midi.h"
#include <SPI.h>

#define FIRST_KEY_NUMBER 43

namespace Keys {
  uint8_t active_oscillators[YM2420_OSCILLATORS] = {0};
  int8_t active_keys[6][7] = {
      {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}};
  uint8_t active_notes[6][7];
  uint8_t last_oscillator = 0;

  void apply() {
    for (uint8_t column = 0; column < 6; column++) {
      uint8_t keys = MCP23S17::scan_matrix(column);

      for (uint8_t row = 0; row < 7; row++) {
        bool key_on = (keys >> row) & 1;

        if (active_keys[column][row] == -1 && key_on) {
          for (uint8_t oscillator = 0; oscillator < YM2420_OSCILLATORS;
               oscillator++) {
            uint8_t check_oscillator = (
                oscillator + last_oscillator + 1) % YM2420_OSCILLATORS;

            if (active_oscillators[check_oscillator] == false) {
              uint8_t note = (
                  check_oscillator, row * 6 + column + FIRST_KEY_NUMBER);

              YM2420::frequency.set_key(check_oscillator, note);
              YM2420::key.set(check_oscillator, true);

              midi.note_on(note);

              last_oscillator = check_oscillator;
              active_oscillators[check_oscillator] = true;
              active_keys[column][row] = check_oscillator;
              active_notes[column][row] = note;

              break;
            }
          }
        } else if (active_keys[column][row] != -1 && ! key_on) {
          YM2420::key.set(active_keys[column][row], false);
          active_oscillators[active_keys[column][row]] = false;
          active_keys[column][row] = -1;

          midi.note_off(active_notes[column][row]);
        }
      }
    }
  }
}
