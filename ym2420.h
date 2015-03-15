#ifndef YM2420_H
#define YM2420_H

#include <stdint.h>  // For uint8_t.
#include <stdbool.h>  // For bool.

#define INSTRUMENT_ORIGINAL 0
#define INSTRUMENT_VIOLIN 1
#define INSTRUMENT_GUITAR 2
#define INSTRUMENT_PIANO 3
#define INSTRUMENT_FLUTE 4
#define INSTRUMENT_CLARINET 5
#define INSTRUMENT_OBOE 6
#define INSTRUMENT_TRUMPET 7
#define INSTRUMENT_ORGAN 8
#define INSTRUMENT_HORN 9
#define INSTRUMENT_SYNTHESIZER 10
#define INSTRUMENT_HARPSICHORD 11
#define INSTRUMENT_VIBRAPHONE 12
#define INSTRUMENT_SYNTHESIZER_BASS 13
#define INSTRUMENT_ACOUSTIC_BASS 14
#define INSTRUMENT_ELECTRIC_GUITAR 15

#define YM2420_OSCILLATORS 8

struct ym2420_bit_t {
  uint8_t address;
  uint8_t bit;
};

struct ym2420_bits_t {
  ym2420_bit_t amplitude_modulation_carrier;
  ym2420_bit_t amplitude_modulation_modulation;
  ym2420_bit_t vibrato_carrier;
  ym2420_bit_t vibrato_modulation;
  ym2420_bit_t sustained_sound_carrier;
  ym2420_bit_t sustained_sound_modulation;
  ym2420_bit_t rate_key_scale_carrier;
  ym2420_bit_t rate_key_scale_modulation;
  ym2420_bit_t wave_distortion_carrier;
  ym2420_bit_t wave_distortion_modulation;
  ym2420_bit_t rhythm_sound;
};

struct ym2420_range_t {
  uint8_t address;
  uint8_t first_bit;
  uint8_t range;
  bool inverted;
};

struct ym2420_ranges_t {
  ym2420_range_t multi_sample_wave_carrier;
  ym2420_range_t multi_sample_wave_modulation;
  ym2420_range_t level_key_scale_carrier;
  ym2420_range_t level_key_scale_modulation;
  ym2420_range_t modulation_index;
  ym2420_range_t fm_feedback_constant;
  ym2420_range_t attack_rate_carrier;
  ym2420_range_t decay_rate_carrier;
  ym2420_range_t sustain_rate_carrier;
  ym2420_range_t release_rate_carrier;
  ym2420_range_t attack_rate_modulation;
  ym2420_range_t decay_rate_modulation;
  ym2420_range_t sustain_rate_modulation;
  ym2420_range_t release_rate_modulation;
  ym2420_range_t rhythm_instruments;
};

struct ym2420_oscillators_t {
  ym2420_bit_t sustain;
  ym2420_bit_t key;
  ym2420_range_t octave;
  ym2420_range_t instrument;
  ym2420_range_t volume;
  ym2420_range_t f_number_0x10;
  ym2420_range_t f_number_0x20;
};

void ym2420_setup();

void ym2420_write_range(ym2420_range_t, uint8_t);
void ym2420_write_bit(ym2420_bit_t, uint8_t);

void ym2420_write_f_number(uint8_t, unsigned int);
void ym2420_write_f_number_key(uint8_t, uint8_t);

extern ym2420_ranges_t ym2420_ranges;
extern ym2420_bits_t ym2420_bits;
extern ym2420_oscillators_t ym2420_oscillators[YM2420_OSCILLATORS];

#endif
