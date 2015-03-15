#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include <Arduino.h>

#define MCP23S17_HW_ADDRESS 0b000

#define YM2420_A0 2
#define YM2420_CS 3

#define YM2420_F_NUMBER_KEYS 88

uint8_t registers[0x38] = {0};
float f_numbers[YM2420_F_NUMBER_KEYS];

ym2420_bits_t ym2420_bits = {
  .amplitude_modulation_carrier = {
    .address = 0x00,
    .bit = 7},

  .amplitude_modulation_modulation = {
    .address = 0x01,
    .bit = 7},

  .vibrato_carrier = {
    .address = 0x00,
    .bit = 6},

  .vibrato_modulation = {
    .address = 0x01,
    .bit = 6},

  .sustained_sound_carrier = {
    .address = 0x00,
    .bit = 5},

  .sustained_sound_modulation = {
    .address = 0x01,
    .bit = 5},

  .rate_key_scale_carrier = {
    .address = 0x00,
    .bit = 4},

  .rate_key_scale_modulation = {
    .address = 0x01,
    .bit = 4},

  .wave_distortion_carrier = {
    .address = 0x03,
    .bit = 3},

  .wave_distortion_modulation = {
    .address = 0x03,
    .bit = 4},

  .rhythm_sound = {
    .address = 0x0e,
    .bit = 5},
};

ym2420_ranges_t ym2420_ranges = {
  .multi_sample_wave_carrier = {
    .address = 0x00,
    .first_bit = 0,
    .range = 15,
    .inverted = false},

  .multi_sample_wave_modulation = {
    .address = 0x01,
    .first_bit = 0,
    .range = 15,
    .inverted = false},

  .level_key_scale_carrier = {
    .address = 0x02,
    .first_bit = 6,
    .range = 3,
    .inverted = true},

  .level_key_scale_modulation = {
    .address = 0x03,
    .first_bit = 6,
    .range = 3,
    .inverted = true},

  .modulation_index = {
    .address = 0x02,
    .first_bit = 0,
    .range = 63,
    .inverted = true},

  .fm_feedback_constant = {
    .address = 0x03,
    .first_bit = 0,
    .range = 7,
    .inverted = false},

  .attack_rate_carrier = {
    .address = 0x04,
    .first_bit = 4,
    .range = 15,
    .inverted = true},

  .decay_rate_carrier = {
    .address = 0x04,
    .first_bit = 0,
    .range = 15,
    .inverted = true},

  .sustain_rate_carrier = {
    .address = 0x06,
    .first_bit = 4,
    .range = 15,
    .inverted = true},

  .release_rate_carrier = {
    .address = 0x06,
    .first_bit = 0,
    .range = 15,
    .inverted = true},

  .attack_rate_modulation = {
    .address = 0x05,
    .first_bit = 4,
    .range = 15,
    .inverted = true},

  .decay_rate_modulation = {
    .address = 0x05,
    .first_bit = 0,
    .range = 15,
    .inverted = true},

  .sustain_rate_modulation = {
    .address = 0x07,
    .first_bit = 4,
    .range = 15,
    .inverted = true},

  .release_rate_modulation = {
    .address = 0x07,
    .first_bit = 0,
    .range = 15,
    .inverted = true},

  .rhythm_instruments = {
    .address = 0x0e,
    .first_bit = 0,
    .range = 31,
    .inverted = false},
};

ym2420_oscillators_t ym2420_oscillators[YM2420_OSCILLATORS];

void ym2420_setup() {
  for (uint8_t key = 0; key < YM2420_F_NUMBER_KEYS; key++) {
    f_numbers[key] = pow(2, (float)(key - 48) / 12) * 288.3584;
  }

  for (uint8_t oscillator = 0; oscillator < YM2420_OSCILLATORS; oscillator++) {
    ym2420_oscillators[oscillator] = {
      .sustain = {
        .address = 0x20 + oscillator,
        .bit = 5},

      .key = {
        .address = 0x20 + oscillator,
        .bit = 4},

      .octave = {
        .address = 0x10 + oscillator,
        .first_bit = 5,
        .range = 7,
        .inverted = false},

      .instrument = {
        .address = 0x30 + oscillator,
        .first_bit = 4,
        .range = 15,
        .inverted = false},

      .volume = {
        .address = 0x30 + oscillator,
        .first_bit = 0,
        .range = 15,
        .inverted = true},

      .f_number_0x10 = {
        .address = 0x10 + oscillator,
        .first_bit = 0,
        .range = 31,
        .inverted = false},

      .f_number_0x20 = {
        .address = 0x20 + oscillator,
        .first_bit = 0,
        .range = 15,
        .inverted = false}
    };
  }

  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x00, 0x00);

  pinMode(YM2420_A0, OUTPUT);
  pinMode(YM2420_CS, OUTPUT);

  digitalWrite(YM2420_A0, 1);
  digitalWrite(YM2420_CS, 1);
}

void ym2420_write(uint8_t address) {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, address);
  digitalWrite(YM2420_A0, 0);
  digitalWrite(YM2420_CS, 0);
  digitalWrite(YM2420_CS, 1);

  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, registers[address]);
  digitalWrite(YM2420_A0, 1);
  digitalWrite(YM2420_CS, 0);
  digitalWrite(YM2420_CS, 1);
}

void ym2420_write_range(ym2420_range_t range, uint8_t value) {
  if (range.inverted) {
    value = range.range - value;
  }

  uint8_t mask = range.range << range.first_bit;
  registers[range.address] = (
      registers[range.address] & ~mask) | (value << range.first_bit & mask);
  ym2420_write(range.address);
}

void ym2420_write_bit(ym2420_bit_t bit, uint8_t value) {
  registers[bit.address] ^= (-value ^ registers[bit.address]) & (1 << bit.bit);
  ym2420_write(bit.address);
}

void ym2420_write_f_number(uint8_t oscillator, unsigned int f_number) {
  ym2420_write_range(
      ym2420_oscillators[oscillator].f_number_0x10, f_number >> 4);
  ym2420_write_range(
      ym2420_oscillators[oscillator].f_number_0x20, f_number & 0b1111);
}

void ym2420_write_f_number_key(uint8_t oscillator, uint8_t key_number) {
  // FIXME: +2?  Why?
  uint8_t octave = ((int)f_numbers[key_number] / 512) + 2;
  unsigned int f_number = f_numbers[key_number] / pow(2, octave);

  ym2420_write_range(ym2420_oscillators[oscillator].octave, octave);
  ym2420_write_f_number(oscillator, f_number);
}
