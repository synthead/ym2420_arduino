#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#define MIDI_CHANNEL_MAX 15
#define MIDI_CC_MAX 127

#define MIDI_DATA_BYTE_MIN 0x00
#define MIDI_DATA_BYTE_MAX 0x7f

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_CC 0xb0

#define MIDI_DEFAULT_CHANNEL 0

#define MIDI_DEFAULT_MULTI_SAMPLE_WAVE_CARRIER_CC       12
#define MIDI_DEFAULT_MULTI_SAMPLE_WAVE_MODULATION_CC    13
#define MIDI_DEFAULT_MODULATION_INDEX_CC                14
#define MIDI_DEFAULT_FM_FEEDBACK_CONSTANT_CC            15
#define MIDI_DEFAULT_LEVEL_KEY_SCALE_CARRIER_CC         16
#define MIDI_DEFAULT_LEVEL_KEY_SCALE_MODULATION_CC      17

#define MIDI_DEFAULT_ATTACK_RATE_CARRIER_CC             18
#define MIDI_DEFAULT_DECAY_RATE_CARRIER_CC              19
#define MIDI_DEFAULT_SUSTAIN_RATE_CARRIER_CC            20
#define MIDI_DEFAULT_RELEASE_RATE_CARRIER_CC            21
#define MIDI_DEFAULT_ATTACK_RATE_MODULATION_CC          22
#define MIDI_DEFAULT_DECAY_RATE_MODULATION_CC           23
#define MIDI_DEFAULT_SUSTAIN_RATE_MODULATION_CC         24
#define MIDI_DEFAULT_RELEASE_RATE_MODULATION_CC         25

#define MIDI_DEFAULT_AMPLITUDE_MODULATION_CARRIER_CC    26
#define MIDI_DEFAULT_VIBRATO_CARRIER_CC                 27
#define MIDI_DEFAULT_WAVE_DISTORTION_CARRIER_CC         28
#define MIDI_DEFAULT_RATE_KEY_SCALE_CARRIER_CC          29
#define MIDI_DEFAULT_AMPLITUDE_MODULATION_MODULATION_CC 30
#define MIDI_DEFAULT_VIBRATO_MODULATION_CC              31
#define MIDI_DEFAULT_WAVE_DISTORTION_MODULATION_CC      32
#define MIDI_DEFAULT_RATE_KEY_SCALE_MODULATION_CC       33

namespace MIDI {
  extern uint8_t channel;

  void setup();
  void panic();
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
