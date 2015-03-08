#ifndef YM2420_H
#define YM2420_H

#define MODULATION 0
#define CARRIER 1

#define MULTI_SAMPLE_WAVE_RANGE 15
#define LEVEL_KEY_SCALE_RANGE 15
#define MODULATION_INDEX_RANGE 63
#define WAVE_DISTORTION_RANGE 3
#define FM_FEEDBACK_CONSTANT_RANGE 7
#define ATTACK_RANGE 15
#define DECAY_RANGE 15
#define SUSTAIN_RANGE 15
#define RELEASE_RANGE 15
#define RHYTHM_INSTRUMENTS_RANGE 31
#define OCTAVE_RANGE 7
#define INSTRUMENT_RANGE 15
#define VOLUME_RANGE 15
#define KEY_RANGE 87

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

void amplitude_modulation(int, int);
void vibrato(int, int);
void sustained_sound(int, int);
void rate_key_scale(int, int);
void multi_sample_wave(int, int);
void level_key_scale(int, int);
void modulation_index(int);
void wave_distortion(int, int);
void fm_feedback_constant(int);
void attack_rate(int, int);
void decay_rate(int, int);
void sustain_rate(int, int);
void release_rate(int, int);
void rhythm_sound(int);
void rhythm_instruments(int);
void sustain(int, int);
void key(int, int);
void octave(int, int);
void instrument(int, int);
void volume(int, int);
void f_number(int, int);
void f_number_key(int, int);
#endif
