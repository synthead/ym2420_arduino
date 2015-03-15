#ifndef YM2420_H
#define YM2420_H

#include <stdint.h>  // For uint8_t.

#define RANGE_MULTI_SAMPLE_WAVE 15
#define RANGE_LEVEL_KEY_SCALE 3
#define RANGE_MODULATION_INDEX 63
#define RANGE_FM_FEEDBACK_CONSTANT 7
#define RANGE_ADSR_RATE 15
#define RANGE_RHYTHM_INSTRUMENTS 31
#define RANGE_OCTAVE 7
#define RANGE_INSTRUMENT 15
#define RANGE_VOLUME 15
#define RANGE_F_NUMBER 511
#define RANGE_F_NUMBER_KEY 87

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

void ym2420_write_changes();
void ym2420_setup();
void ym2420_reset();

void amplitude_modulation_carrier(bool);
void amplitude_modulation_modulation(bool);
void vibrato_carrier(bool);
void vibrato_modulation(bool);
void sustained_sound_carrier(bool);
void sustained_sound_modulation(bool);
void rate_key_scale_carrier(bool);
void rate_key_scale_modulation(bool);
void multi_sample_wave_carrier(uint8_t);
void multi_sample_wave_modulation(uint8_t);
void level_key_scale_carrier(uint8_t);
void level_key_scale_modulation(uint8_t);
void modulation_index(uint8_t);
void wave_distortion_carrier(bool);
void wave_distortion_modulation(bool);
void fm_feedback_constant(uint8_t);
void attack_rate_carrier(uint8_t);
void decay_rate_carrier(uint8_t);
void sustain_rate_carrier(uint8_t);
void release_rate_carrier(uint8_t);
void attack_rate_modulation(uint8_t);
void decay_rate_modulation(uint8_t);
void sustain_rate_modulation(uint8_t);
void release_rate_modulation(uint8_t);
void rhythm_sound(bool);
void rhythm_instruments(uint8_t);
void sustain(uint8_t, bool);
void key(uint8_t, bool);
void octave(uint8_t, uint8_t);
void instrument(uint8_t, uint8_t);
void volume(uint8_t, uint8_t);
void f_number(uint8_t, unsigned int);
void f_number_key(uint8_t, uint8_t);

#endif
