#ifndef YM2420_H
#define YM2420_H

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

void write_ym2420_changes();
void setup_ym2420();
void reset_ym2420();

void toggle_oscillator(int);
void f_number(int, int);
void am_toggle(int);
void vibrato_toggle(int);
void sustained_sound_toggle(int);
void rate_key_scale_toggle(int);
void multi_sample_wave(int, int);
void level_key_scale(int, int);
void modulation_index(int);
void wave_distortion(int);
void fm_feedback_constant(int);
void attack(int, int);
void decay(int, int);
void sustain(int, int);
void release(int, int);
void rhythm_sound_toggle(int);
void rhythm_instruments(int);
void sustain_toggle(int);
void key_toggle(int);
void octave(int, int);
void instrument(int, int);
void volume(int, int);
void key(int, int);

#endif
