#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include <Arduino.h>

#define MCP23S17_HW_ADDRESS 0b000

#define YM_A0 2
#define YM_CS 3
#define YM_IC 4

#define REGISTER_CHANGES_MAX 100
#define RESET_TIME 1000

uint8_t registers[0x38] = {0};
uint8_t register_changes[REGISTER_CHANGES_MAX];
uint8_t register_changes_sum = 0;

// Generate this data by compiling the blub below like so:
// $ gcc foo.c -o foo -lm -std=c99

// #include <stdio.h>
// #include <math.h>
//
// int main() {
//   for (int i = 0; i < 88; i++) {
//     printf("%f\n", pow(2, (float)(i - 48) / 12) * 288.3584);
//   }
// }

float f_numbers[88] = {
    18.022400, 19.094067, 20.229461, 21.432366, 22.706800, 24.057019,
    25.487523, 27.003088, 28.608778, 30.309943, 32.112264, 34.021763,
    36.044800, 38.188133, 40.458922, 42.864733, 45.413600, 48.114038,
    50.975045, 54.006176, 57.217557, 60.619886, 64.224529, 68.043525,
    72.089600, 76.376273, 80.917838, 85.729465, 90.827207, 96.228069,
    101.950090, 108.012361, 114.435104, 121.239772, 128.449068, 136.087040,
    144.179200, 152.752539, 161.835682, 171.458930, 181.654407, 192.456145,
    203.900180, 216.024717, 228.870212, 242.479545, 256.898128, 272.174086,
    288.358400, 305.505083, 323.671361, 342.917861, 363.308821, 384.912282,
    407.800360, 432.049425, 457.740434, 484.959090, 513.796251, 544.348181,
    576.716800, 611.010182, 647.342703, 685.835722, 726.617656, 769.824548,
    815.600720, 864.098887, 915.480830, 969.918179, 1027.592544, 1088.696316,
    1153.433600, 1222.020264, 1294.685513, 1371.671444, 1453.235192,
    1539.649223, 1631.201440, 1728.197630, 1830.961811, 1939.836359,
    2055.184918, 2177.392813, 2306.867200, 2444.040527, 2589.371025,
    2743.342888};

void ym2420_setup() {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x00, 0x00);

  pinMode(YM_A0, OUTPUT);
  pinMode(YM_CS, OUTPUT);
  pinMode(YM_IC, OUTPUT);

  digitalWrite(YM_A0, 1);
  digitalWrite(YM_CS, 1);
  digitalWrite(YM_IC, 1);

  ym2420_reset();
}

void ym2420_reset() {
  digitalWrite(YM_IC, 0);
  delayMicroseconds(RESET_TIME);
  digitalWrite(YM_IC, 1);
}

void ym2420_write_changes() {
  for (uint8_t change = 0; change < register_changes_sum; change++) {
    mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, register_changes[change]);
    digitalWrite(YM_A0, 0);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);

    mcp23s17_write(
        MCP23S17_HW_ADDRESS, 0x12, registers[register_changes[change]]);
    digitalWrite(YM_A0, 1);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);
  }

  register_changes_sum = 0;
}

void store_byte(uint8_t address, uint8_t new_value) {
  registers[address] = new_value;
  register_changes[register_changes_sum] = address;
  register_changes_sum++;
}

void store_bit(uint8_t address, bool value, uint8_t bit) {
  uint8_t new_value = registers[address] ^ (
      -value ^ registers[address]) & (1 << bit);
  store_byte(address, new_value);
}

void store_range(
    uint8_t address, uint8_t value, uint8_t position, uint8_t length) {
  uint8_t mask = ((1 << length) - 1) << position;
  uint8_t new_value = (registers[address] & ~mask) | (value << position & mask);
  store_byte(address, new_value);
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
