#include "fm_synth.h"
#include "registers.h"
#include "f_numbers.h"

void toggle_oscillator(int oscillator) {
  toggle_register_bit(0x20 + oscillator, 4);
}

void f_number(int oscillator, int f_number) {
  store_register_range(0x20 + oscillator, f_number & 0b000001111, 0, 4);
  store_register_range(0x10 + oscillator, f_number >> 4, 0, 5);
}

void am_toggle(int oscillator) {
  toggle_register_bit(0x00 + oscillator, 7);
}

void vibrato_toggle(int oscillator) {
  toggle_register_bit(0x00 + oscillator, 6);
}

void sustained_sound_toggle(int oscillator) {
  toggle_register_bit(0x00 + oscillator, 5);
}

void rate_key_scale_toggle(int oscillator) {
  toggle_register_bit(0x00 + oscillator, 4);
}

void multi_sample_wave(int oscillator, int value) {
  store_register_range(0x00 + oscillator, value, 0, 4);
}

void level_key_scale(int oscillator, int value) {
  store_register_range(0x02 + oscillator, value, 6, 2);
}

void modulation_index(int value) {
  store_register_range(0x02, value, 0, 6);
}

void wave_distortion(int value) {
  store_register_range(0x03, value, 3, 2);
}

void fm_feedback_constant(int value) {
  store_register_range(0x03, value, 0, 3);
}

void attack(int oscillator, int value) {
  store_register_range(0x04 + oscillator, value, 4, 4);
}

void decay(int oscillator, int value) {
  store_register_range(0x04 + oscillator, value, 0, 4);
}

void sustain(int oscillator, int value) {
  store_register_range(0x06 + oscillator, value, 4, 4);
}

void release(int oscillator, int value) {
  store_register_range(0x06 + oscillator, value, 0, 4);
}

void rhythm_sound_toggle(int value) {
  toggle_register_bit(0x0E, 5);
}

void rhythm_instruments(int value) {
  store_register_range(0x0e, value, 0, 5);
}

void sustain_toggle(int oscillator) {
  toggle_register_bit(0x20 + oscillator, 5);
}

void key_toggle(int oscillator) {
  toggle_register_bit(0x20 + oscillator, 4);
}

void octave(int oscillator, int value) {
  store_register_range(0x10 + oscillator, value, 5, 3);
}

void instrument(int oscillator, int value) {
  store_register_range(0x30 + oscillator, value, 4, 4);
}

void volume(int oscillator, int value) {
  store_register_range(0x30 + oscillator, value, 0, 4);
}

void key(int oscillator, int key_number) {
  int octave_number = (int)f_numbers[key_number] / 512;
  int f_number_int = f_numbers[key_number] / pow(2, octave_number);

  octave(0, octave_number + 2);
  f_number(oscillator, f_number_int);
}
