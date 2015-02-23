#ifndef FM_SYNTH_H
#define FM_SYNTH_H

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
