#ifndef YM2420_H
#define YM2420_H

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

void amplitude_modulation_carrier(int);
void amplitude_modulation_modulation(int);
void vibrato_carrier(int);
void vibrato_modulation(int);
void sustained_sound_carrier(int);
void sustained_sound_modulation(int);
void rate_key_scale_carrier(int);
void rate_key_scale_modulation(int);
void multi_sample_wave_carrier(int);
void multi_sample_wave_modulation(int);
void level_key_scale_carrier(int);
void level_key_scale_modulation(int);
void modulation_index(int);
void wave_distortion_carrier(int);
void wave_distortion_modulation(int);
void fm_feedback_constant(int);
void attack_rate_carrier(int);
void decay_rate_carrier(int);
void sustain_rate_carrier(int);
void release_rate_carrier(int);
void attack_rate_modulation(int);
void decay_rate_modulation(int);
void sustain_rate_modulation(int);
void release_rate_modulation(int);
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
