#include "midi.h"
#include "ym2420.h"
#include "controls.h"
#include "storage.h"
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
    uint8_t multi_sample_wave_carrier;
    uint8_t multi_sample_wave_modulation;
    uint8_t modulation_index;
    uint8_t fm_feedback_constant;
    uint8_t level_key_scale_carrier;
    uint8_t level_key_scale_modulation;

    uint8_t attack_rate_carrier;
    uint8_t decay_rate_carrier;
    uint8_t sustain_rate_carrier;
    uint8_t release_rate_carrier;
    uint8_t attack_rate_modulation;
    uint8_t decay_rate_modulation;
    uint8_t sustain_rate_modulation;
    uint8_t release_rate_modulation;

    uint8_t amplitude_modulation_carrier;
    uint8_t vibrato_carrier;
    uint8_t wave_distortion_carrier;
    uint8_t rate_key_scale_carrier;
    uint8_t amplitude_modulation_modulation;
    uint8_t vibrato_modulation;
    uint8_t wave_distortion_modulation;
    uint8_t rate_key_scale_modulation;
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
              ym2420_oscillator_off(parameter_1_in);
            }
            break;
          case MIDI_NOTE_OFF:
            ym2420_oscillator_off(parameter_1_in);
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
