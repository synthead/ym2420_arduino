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

#define YM2420_F_NUMBER_KEYS 88
#define YM2420_OSCILLATORS 8

void ym2420_setup();

class YM2420Bit {
    const uint8_t address;
    const uint8_t bit_number;
  public:
    YM2420Bit(uint8_t, uint8_t);
    void set(uint8_t);
};

class YM2420Range {
    const uint8_t address;
    const uint8_t first_bit;
    const uint8_t range;
    const bool inverted;
  public:
    YM2420Range(uint8_t, uint8_t, uint8_t, bool);
    uint8_t get_range();
    void set(uint8_t);
};

class YM2420OscillatorBit {
    const uint8_t address;
    const uint8_t bit_number;
  public:
    YM2420OscillatorBit(uint8_t, uint8_t);
    void set(uint8_t, uint8_t);
};

class YM2420OscillatorRange {
    uint8_t address;
    uint8_t first_bit;
    uint8_t range;
    bool inverted;
  public:
    YM2420OscillatorRange(uint8_t, uint8_t, uint8_t, bool);
    uint8_t get_range();
    void set(uint8_t, uint8_t);
};

class YM2420OscillatorFrequency {
    YM2420OscillatorRange *octave;

    uint8_t address_lsb;
    uint8_t first_bit_lsb;
    uint8_t range_lsb;

    uint8_t address_msb;
    uint8_t first_bit_msb;
    uint8_t range_msb;

    float f_numbers[YM2420_F_NUMBER_KEYS];
  public:
    YM2420OscillatorFrequency(
        YM2420OscillatorRange*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
        uint8_t);
    void set_f_number(uint8_t, unsigned int);
    void set_key(uint8_t, uint8_t);
};

extern YM2420Bit amplitude_modulation_carrier;
extern YM2420Bit amplitude_modulation_modulation;
extern YM2420Bit vibrato_carrier;
extern YM2420Bit vibrato_modulation;
extern YM2420Bit sustained_sound_carrier;
extern YM2420Bit sustained_sound_modulation;
extern YM2420Bit rate_key_scale_carrier;
extern YM2420Bit rate_key_scale_modulation;
extern YM2420Bit wave_distortion_carrier;
extern YM2420Bit wave_distortion_modulation;
extern YM2420Bit rhythm_sound;

extern YM2420Range multi_sample_wave_carrier;
extern YM2420Range multi_sample_wave_modulation;
extern YM2420Range level_key_scale_carrier;
extern YM2420Range level_key_scale_modulation;
extern YM2420Range modulation_index;
extern YM2420Range fm_feedback_constant;
extern YM2420Range attack_rate_carrier;
extern YM2420Range decay_rate_carrier;
extern YM2420Range sustain_rate_carrier;
extern YM2420Range release_rate_carrier;
extern YM2420Range attack_rate_modulation;
extern YM2420Range decay_rate_modulation;
extern YM2420Range sustain_rate_modulation;
extern YM2420Range release_rate_modulation;
extern YM2420Range rhythm_instruments;

extern YM2420OscillatorBit sustain;
extern YM2420OscillatorBit key;

extern YM2420OscillatorRange octave;
extern YM2420OscillatorRange instrument;
extern YM2420OscillatorRange volume;

extern YM2420OscillatorFrequency frequency;

#endif
