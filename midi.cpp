#include "midi.h"
#include "ym2420.h"
#include <Arduino.h>

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90

#define MIDI_NOTE_OFF_VELOCITY 0x00
#define MIDI_NOTE_ON_VELOCITY 0x7f

#define MIDI_CHANNEL_MAX 15

namespace MIDI {
  uint8_t channel = 0;
  uint8_t command = 0;
  int8_t parameter_1 = -1;

  void next_channel() {
    if (channel < MIDI_CHANNEL_MAX) {
      channel++;
    }
  }

  void previous_channel() {
    if (channel > 0) {
      channel--;
    }
  }

  void write_command(uint8_t command) {
    Serial1.write(command + channel);
  }

  void key_on(uint8_t key) {
    write_command(MIDI_NOTE_ON);
    Serial1.write(key);
    Serial1.write(MIDI_NOTE_ON_VELOCITY);
  }

  void key_off(uint8_t key) {
    write_command(MIDI_NOTE_OFF);
    Serial1.write(key);
    Serial1.write(MIDI_NOTE_OFF_VELOCITY);
  }

  void process_messages() {
    while (Serial1.available() > 0) {
      uint8_t midi_data = Serial1.read();

      if (midi_data >= MIDI_NOTE_OFF) {
        uint8_t received_channel = midi_data % 0x10;

        if (received_channel == channel) {
          command = midi_data - channel;
        }
      } else if (parameter_1 == -1) {
        switch (command) {
          case MIDI_NOTE_ON:
          case MIDI_NOTE_OFF:
            parameter_1 = midi_data;
            break;
        }
      } else {
        switch (command) {
          case MIDI_NOTE_ON:
            if (midi_data > 0x00) {
              YM2420::key_on(parameter_1);
            } else {
              YM2420::key_off(parameter_1);
            }
            break;
          case MIDI_NOTE_OFF:
            YM2420::key_off(parameter_1);
            break;
        }

        parameter_1 = -1;
      }
    }
  }
}
