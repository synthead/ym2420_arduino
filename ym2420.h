#ifndef YM2420_H
#define YM2420_H

#include <stdint.h>
#include <stdbool.h>

#define YM2420_F_NUMBER_KEYS 88
#define YM2420_OSCILLATORS 9
#define YM2420_REGISTERS 0x38

#define YM2420_PATCH_BYTES 8
#define YM2420_NO_OSCILLATOR YM2420_OSCILLATORS

namespace YM2420 {
  void setup();
  void write(uint8_t);
  uint8_t oscillator_on(uint8_t, uint8_t);
  void oscillator_off(uint8_t);
  void panic();

  class Bit {
      const uint8_t address;
      const uint8_t bit_number;
    public:
      Bit(uint8_t, uint8_t);
      void set(uint8_t);
  };

  class Range {
      const uint8_t address;
      const uint8_t first_bit;
      const uint8_t range;
      const bool inverted;
    public:
      Range(uint8_t, uint8_t, uint8_t, bool);
      uint8_t get_range();
      void set(uint8_t);
  };

  class OscillatorBit {
      const uint8_t address;
      const uint8_t bit_number;
    public:
      OscillatorBit(uint8_t, uint8_t);
      void set(uint8_t, uint8_t);
  };

  class OscillatorRange {
      uint8_t address;
      uint8_t first_bit;
      uint8_t range;
      bool inverted;
    public:
      OscillatorRange(uint8_t, uint8_t, uint8_t, bool);
      uint8_t get_range();
      void set(uint8_t, uint8_t);
  };

  extern uint8_t register_contents[YM2420_REGISTERS];

  extern Bit amplitude_modulation_carrier;
  extern Bit amplitude_modulation_modulation;
  extern Bit vibrato_carrier;
  extern Bit vibrato_modulation;
  extern Bit sustained_sound_carrier;
  extern Bit sustained_sound_modulation;
  extern Bit rate_key_scale_carrier;
  extern Bit rate_key_scale_modulation;
  extern Bit wave_distortion_carrier;
  extern Bit wave_distortion_modulation;
  extern Bit rhythm_sound;

  extern Range multi_sample_wave_carrier;
  extern Range multi_sample_wave_modulation;
  extern Range level_key_scale_carrier;
  extern Range level_key_scale_modulation;
  extern Range modulation_index;
  extern Range fm_feedback_constant;
  extern Range attack_rate_carrier;
  extern Range decay_rate_carrier;
  extern Range sustain_rate_carrier;
  extern Range release_rate_carrier;
  extern Range attack_rate_modulation;
  extern Range decay_rate_modulation;
  extern Range sustain_rate_modulation;
  extern Range release_rate_modulation;
  extern Range rhythm_instruments;

  extern OscillatorBit sustain;
  extern OscillatorBit key;

  extern OscillatorRange octave;
  extern OscillatorRange instrument;
  extern OscillatorRange volume;
  extern OscillatorRange f_number_lsb;
  extern OscillatorRange f_number_msb;
}

#endif
