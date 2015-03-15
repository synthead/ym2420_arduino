#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include <Arduino.h>

#define MCP23S17_HW_ADDRESS 0b000

#define YM_A0 2
#define YM_CS 3

uint8_t registers[0x38] = {0};
float f_numbers[88];

void ym2420_setup() {
  for (uint8_t key = 0; key < 88; key++) {
    f_numbers[key] = pow(2, (float)(key - 48) / 12) * 288.3584;
  }

  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x00, 0x00);

  pinMode(YM_A0, OUTPUT);
  pinMode(YM_CS, OUTPUT);

  digitalWrite(YM_A0, 1);
  digitalWrite(YM_CS, 1);
}

void ym2420_write(uint8_t address) {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, address);
  digitalWrite(YM_A0, 0);
  digitalWrite(YM_CS, 0);
  digitalWrite(YM_CS, 1);

  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, registers[address]);
  digitalWrite(YM_A0, 1);
  digitalWrite(YM_CS, 0);
  digitalWrite(YM_CS, 1);
}

void store_bit(uint8_t address, bool value, uint8_t bit) {
  registers[address] ^= (-value ^ registers[address]) & (1 << bit);
  ym2420_write(address);
}

void store_range(
    uint8_t address, uint8_t value, uint8_t position, uint8_t length) {
  uint8_t mask = ((1 << length) - 1) << position;
  registers[address] = (
      registers[address] & ~mask) | (value << position & mask);
  ym2420_write(address);
}

void amplitude_modulation_carrier(bool value) {
  store_bit(0x00, value, 7);
}

void amplitude_modulation_modulation(bool value) {
  store_bit(0x01, value, 7);
}

void vibrato_carrier(bool value) {
  store_bit(0x00, value, 6);
}

void vibrato_modulation(bool value) {
  store_bit(0x01, value, 6);
}

void sustained_sound_carrier(bool value) {
  store_bit(0x00, value, 5);
}

void sustained_sound_modulation(bool value) {
  store_bit(0x01, value, 5);
}

void rate_key_scale_carrier(bool value) {
  store_bit(0x00, value, 4);
}

void rate_key_scale_modulation(bool value) {
  store_bit(0x01, value, 4);
}

void multi_sample_wave_carrier(uint8_t value) {
  store_range(0x00, value, 0, 4);
}

void multi_sample_wave_modulation(uint8_t value) {
  store_range(0x01, value, 0, 4);
}

void level_key_scale_carrier(uint8_t value) {
  value = RANGE_LEVEL_KEY_SCALE - value;
  store_range(0x02, value, 6, 2);
}

void level_key_scale_modulation(uint8_t value) {
  value = RANGE_LEVEL_KEY_SCALE - value;
  store_range(0x03, value, 6, 2);
}

void modulation_index(uint8_t value) {
  value = RANGE_MODULATION_INDEX - value;
  store_range(0x02, value, 0, 6);
}

void wave_distortion_carrier(bool value) {
  store_bit(0x03, value, 3);
}

void wave_distortion_modulation(bool value) {
  store_bit(0x03, value, 4);
}

void fm_feedback_constant(uint8_t value) {
  store_range(0x03, value, 0, 3);
}

void attack_rate_carrier(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x04, value, 4, 4);
}

void decay_rate_carrier(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x04, value, 0, 4);
}

void sustain_rate_carrier(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x06, value, 4, 4);
}

void release_rate_carrier(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x06, value, 0, 4);
}

void attack_rate_modulation(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x05, value, 4, 4);
}

void decay_rate_modulation(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x05, value, 0, 4);
}

void sustain_rate_modulation(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x07, value, 4, 4);
}

void release_rate_modulation(uint8_t value) {
  value = RANGE_ADSR_RATE - value;
  store_range(0x07, value, 0, 4);
}

void rhythm_sound(bool value) {
  store_bit(0x0e, value, 5);
}

void rhythm_instruments(uint8_t value) {
  store_range(0x0e, value, 0, 5);
}

void sustain(uint8_t oscillator, bool value) {
  store_bit(0x20 + oscillator, value, 5);
}

void key(uint8_t oscillator, bool value) {
  store_bit(0x20 + oscillator, value, 4);
}

void octave(uint8_t oscillator, uint8_t value) {
  store_range(0x10 + oscillator, value, 5, 3);
}

void instrument(uint8_t oscillator, uint8_t value) {
  store_range(0x30 + oscillator, value, 4, 4);
}

void volume(uint8_t oscillator, uint8_t value) {
  store_range(0x30 + oscillator, value, 0, 4);
}

void f_number(uint8_t oscillator, unsigned int f_number) {
  store_range(0x20 + oscillator, f_number & 0b000001111, 0, 4);
  store_range(0x10 + oscillator, f_number >> 4, 0, 5);
}

void f_number_key(uint8_t oscillator, uint8_t key_number) {
  uint8_t octave_number = (int)f_numbers[key_number] / 512;
  unsigned int f_number_int = f_numbers[key_number] / pow(2, octave_number);

  octave(oscillator, octave_number + 2);
  f_number(oscillator, f_number_int);
}
