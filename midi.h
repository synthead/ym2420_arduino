#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

namespace MIDI {
  extern uint8_t channel;
  void key_on(uint8_t);
  void key_off(uint8_t);
  void process_messages();
}

#endif
