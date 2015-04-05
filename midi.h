#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#define MIDI_DATA_BYTE_MIN 0x00
#define MIDI_DATA_BYTE_MAX 0x7f

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_CONTROLLER 0xb0

namespace MIDI {
  extern uint8_t channel;

  void next_channel();
  void previous_channel();
  void send(uint8_t, uint8_t, uint8_t);
  void process_messages();
}

#endif
