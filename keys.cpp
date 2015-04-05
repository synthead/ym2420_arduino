#include "keys.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "midi.h"

#define KEY_COLUMNS 6
#define KEY_ROWS 7

#define KEY_OFF_VELOCITY MIDI_DATA_BYTE_MIN
#define KEY_ON_VELOCITY MIDI_DATA_BYTE_MAX

namespace Keys {
  uint8_t keys[KEY_COLUMNS] = {0};

  void apply() {
    for (uint8_t column = 0; column < KEY_COLUMNS; column++) {
      uint8_t last_keys = keys[column];
      keys[column] = MCP23S17::scan_matrix(column);
      uint8_t key_changes = keys[column] ^ last_keys;

      if (key_changes) {
        for (uint8_t row = 0; row < KEY_ROWS; row++) {
          if ((key_changes >> row) & 0b1) {
            uint8_t key = row * 6 + column + 7;

            if ((keys[column] >> row) & 0b1) {
              YM2420::key_on(key, YM2420::volume.get_range());
              MIDI::send(MIDI_NOTE_ON, key, KEY_ON_VELOCITY);
            } else {
              YM2420::key_off(key);
              MIDI::send(MIDI_NOTE_ON, key, KEY_OFF_VELOCITY);
            }
          }
        }
      }
    }
  }
}
