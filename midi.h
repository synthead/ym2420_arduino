#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#define MIDI_DATA_BYTE_MIN 0x00
#define MIDI_DATA_BYTE_MAX 0x7f

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_CC 0xb0

namespace MIDI {
  extern uint8_t channel;

  void next_channel();
  void previous_channel();
  void send(uint8_t, uint8_t, uint8_t);
  void process_messages();

  namespace CC {
    extern uint8_t multi_sample_wave_carrier;
    extern uint8_t multi_sample_wave_modulation;
    extern uint8_t modulation_index;
    extern uint8_t fm_feedback_constant;
    extern uint8_t level_key_scale_carrier;
    extern uint8_t level_key_scale_modulation;

    extern uint8_t attack_rate_carrier;
    extern uint8_t decay_rate_carrier;
    extern uint8_t sustain_rate_carrier;
    extern uint8_t release_rate_carrier;
    extern uint8_t attack_rate_modulation;
    extern uint8_t decay_rate_modulation;
    extern uint8_t sustain_rate_modulation;
    extern uint8_t release_rate_modulation;

    extern uint8_t amplitude_modulation_carrier;
    extern uint8_t vibrato_carrier;
    extern uint8_t wave_distortion_carrier;
    extern uint8_t rate_key_scale_carrier;
    extern uint8_t amplitude_modulation_modulation;
    extern uint8_t vibrato_modulation;
    extern uint8_t wave_distortion_modulation;
    extern uint8_t rate_key_scale_modulation;
  }
}

#endif
