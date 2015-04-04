#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#define MIDI_DATA_BYTE_MIN 0x00
#define MIDI_DATA_BYTE_MAX 0x7f

namespace MIDI {
  extern uint8_t channel;

  void next_channel();
  void previous_channel();
  void key_on(uint8_t, uint8_t);
  void key_off(uint8_t, uint8_t);
  void process_messages();
}

#endif
