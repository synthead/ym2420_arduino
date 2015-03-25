#include "midi.h"
#include <Arduino.h>

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON_VELOCITY 0x7f
#define MIDI_NOTE_OFF_VELOCITY 0x00

MIDI::MIDI(uint8_t channel) {
  set_channel(channel);
}

void MIDI::write_command(uint8_t command) {
  Serial1.write(command + channel);
}

void MIDI::set_channel(uint8_t new_channel) {
  channel = new_channel - 1;
}

void MIDI::note_on(uint8_t note) {
  write_command(MIDI_NOTE_ON);
  Serial1.write(note);
  Serial1.write(MIDI_NOTE_ON_VELOCITY);
}

void MIDI::note_off(uint8_t note) {
  write_command(MIDI_NOTE_OFF);
  Serial1.write(note);
  Serial1.write(MIDI_NOTE_OFF_VELOCITY);
}

MIDI midi(1);
