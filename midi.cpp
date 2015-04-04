#include "midi.h"
#include "ym2420.h"
#include <Arduino.h>

#define MIDI_CHANNEL_MAX 15
#define MIDI_DIGITAL_HIGH 64

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_CONTROLLER 0xb0

#define MIDI_CC_ANALOG_MIN 12
#define MIDI_CC_ANALOG_MAX 25
#define MIDI_CC_DIGITAL_MIN 26
#define MIDI_CC_DIGITAL_MAX 33

namespace MIDI {
  uint8_t channel = 0;
  uint8_t command = 0;
  int8_t parameter_1 = -1;

  YM2420::Range* analog_controls[] = {
      &YM2420::multi_sample_wave_carrier,
      &YM2420::multi_sample_wave_modulation,
      &YM2420::modulation_index,
      &YM2420::fm_feedback_constant,
      &YM2420::level_key_scale_carrier,
      &YM2420::level_key_scale_modulation,

      &YM2420::attack_rate_carrier,
      &YM2420::decay_rate_carrier,
      &YM2420::sustain_rate_carrier,
      &YM2420::release_rate_carrier,
      &YM2420::attack_rate_modulation,
      &YM2420::decay_rate_modulation,
      &YM2420::sustain_rate_modulation,
      &YM2420::release_rate_modulation
  };

  YM2420::Bit* digital_controls[] = {
      &YM2420::amplitude_modulation_carrier,
      &YM2420::vibrato_carrier,
      &YM2420::wave_distortion_carrier,
      &YM2420::rate_key_scale_carrier,
      &YM2420::amplitude_modulation_modulation,
      &YM2420::vibrato_modulation,
      &YM2420::wave_distortion_modulation,
      &YM2420::rate_key_scale_modulation
  };

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

  void key_on(uint8_t key, uint8_t velocity) {
    write_command(MIDI_NOTE_ON);
    Serial1.write(key);
    Serial1.write(velocity);
  }

  void key_off(uint8_t key, uint8_t velocity) {
    write_command(MIDI_NOTE_OFF);
    Serial1.write(key);
    Serial1.write(velocity);
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
          case MIDI_NOTE_OFF:
          case MIDI_NOTE_ON:
          case MIDI_CONTROLLER:
            parameter_1 = midi_data;
            break;
        }
      } else {
        switch (command) {
          case MIDI_NOTE_ON:
            if (midi_data > 0x00) {
              YM2420::key_on(
                  parameter_1,
                  midi_data * YM2420::volume.get_range() / MIDI_DATA_BYTE_MAX);
            } else {
              YM2420::key_off(parameter_1);
            }
            break;
          case MIDI_NOTE_OFF:
            YM2420::key_off(parameter_1);
            break;
          case MIDI_CONTROLLER:
            if (parameter_1 >= MIDI_CC_ANALOG_MIN &&
                parameter_1 <= MIDI_CC_ANALOG_MAX) {
              YM2420::Range* range = analog_controls[
                  parameter_1 - MIDI_CC_ANALOG_MIN];
              range->set(midi_data * range->get_range() / MIDI_DATA_BYTE_MAX);
            } else if (parameter_1 >= MIDI_CC_DIGITAL_MIN &&
                       parameter_1 <= MIDI_CC_DIGITAL_MAX) {
              digital_controls[parameter_1 - MIDI_CC_DIGITAL_MIN]->set(
                  midi_data >= 64);
            }

            break;
        }

        parameter_1 = -1;
      }
    }
  }
}
