#include "midi.h"
#include "ym2420.h"
#include "controls.h"
#include <Arduino.h>

#define MIDI_CHANNEL_MAX 15
#define MIDI_DIGITAL_HIGH 64

#define MIDI_CC_ANALOG_MIN 12
#define MIDI_CC_ANALOG_MAX 25
#define MIDI_CC_DIGITAL_MIN 26
#define MIDI_CC_DIGITAL_MAX 33

namespace MIDI {
  uint8_t channel = 0;
  uint8_t last_command_out = 0;
  uint8_t command_in = 0;
  int8_t parameter_1_in = -1;
  uint8_t oscillators[MIDI_DATA_BYTE_MAX] = {0};

  namespace CC {
    uint8_t multi_sample_wave_carrier       = 12;
    uint8_t multi_sample_wave_modulation    = 13;
    uint8_t modulation_index                = 14;
    uint8_t fm_feedback_constant            = 15;
    uint8_t level_key_scale_carrier         = 16;
    uint8_t level_key_scale_modulation      = 17;

    uint8_t attack_rate_carrier             = 18;
    uint8_t decay_rate_carrier              = 19;
    uint8_t sustain_rate_carrier            = 20;
    uint8_t release_rate_carrier            = 21;
    uint8_t attack_rate_modulation          = 22;
    uint8_t decay_rate_modulation           = 23;
    uint8_t sustain_rate_modulation         = 24;
    uint8_t release_rate_modulation         = 25;

    uint8_t amplitude_modulation_carrier    = 26;
    uint8_t vibrato_carrier                 = 27;
    uint8_t wave_distortion_carrier         = 28;
    uint8_t rate_key_scale_carrier          = 29;
    uint8_t amplitude_modulation_modulation = 30;
    uint8_t vibrato_modulation              = 31;
    uint8_t wave_distortion_modulation      = 32;
    uint8_t rate_key_scale_modulation       = 33;
  }

  void setup() {
    for (uint8_t oscillator = 0; oscillator < MIDI_DATA_BYTE_MAX;
         oscillator++) {
      oscillators[oscillator] = YM2420_NO_OSCILLATOR;
    }
  }

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

  void send(uint8_t command, uint8_t parameter_1_in, uint8_t parameter_2) {
    if (command != last_command_out) {
      Serial1.write(command + channel);
      last_command_out = command;
    }

    Serial1.write(parameter_1_in);
    Serial1.write(parameter_2);
  }

  void ym2420_range_set_midi(YM2420::Range* range, uint8_t midi_value) {
    range->set(midi_value * range->get_range() / MIDI_DATA_BYTE_MAX);
  }

  void ym2420_bit_set_midi(YM2420::Bit* bit, uint8_t midi_value) {
    bit->set(midi_value >= 64);
  }

  void ym2420_oscillator_off(int8_t parameter_1_in) {
    if (oscillators[parameter_1_in] != YM2420_NO_OSCILLATOR) {
      YM2420::oscillator_off(oscillators[parameter_1_in]);
      oscillators[parameter_1_in] = YM2420_NO_OSCILLATOR;
    }
  }

  void process_messages() {
    while (Serial1.available() > 0) {
      uint8_t midi_data = Serial1.read();

      if (midi_data >= MIDI_NOTE_OFF) {
        uint8_t received_channel = midi_data % 0x10;

        if (received_channel == channel) {
          command_in = midi_data - channel;
        }
      } else if (parameter_1_in == -1) {
        switch (command_in) {
          case MIDI_NOTE_OFF:
          case MIDI_NOTE_ON:
          case MIDI_CC:
            parameter_1_in = midi_data;
            break;
        }
      } else {
        switch (command_in) {
          case MIDI_NOTE_ON:
            if (midi_data > 0x00) {
              oscillators[parameter_1_in] = YM2420::oscillator_on(
                  parameter_1_in,
                  midi_data * YM2420::volume.get_range() / MIDI_DATA_BYTE_MAX);
            } else {
              MIDI::ym2420_oscillator_off(parameter_1_in);
            }
            break;
          case MIDI_NOTE_OFF:
            MIDI::ym2420_oscillator_off(parameter_1_in);
            break;
          case MIDI_CC:
            bool cc_found = false;

            for (uint8_t control = 0; control <= ANALOG_CONTROL_COUNT;
                 control++) {
              if (Controls::analog_controls[control].set_via_cc_number(
                  parameter_1_in, midi_data)) {
                cc_found = true;
                break;
              }
            }

            if (! cc_found) {
              for (uint8_t control = 0; control <= DIGITAL_CONTROL_COUNT;
                   control++) {
                if (Controls::digital_controls[control].set_via_cc_number(
                    parameter_1_in, midi_data)) {
                  break;
                }
              }
            }

            break;
        }

        parameter_1_in = -1;
      }
    }
  }
}
