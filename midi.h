#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

class MIDI {
    uint8_t channel;
    void write_command(uint8_t);
  public:
    MIDI(uint8_t);
    void set_channel(uint8_t);
    void note_on(uint8_t);
    void note_off(uint8_t);
};

extern MIDI midi;

#endif
