#include "ym2420.h"
#include <Arduino.h>

#define YM_D0 0
#define YM_D1 1
#define YM_D2 2
#define YM_D3 3
#define YM_D4 4
#define YM_D5 5
#define YM_D6 6
#define YM_D7 7

#define YM_A0 8
#define YM_CS 9
#define YM_IC 10

#define REGISTER_CHANGES_MAX 100
#define RESET_TIME 1000

int YM_D[8] = {YM_D0, YM_D1, YM_D2, YM_D3, YM_D4, YM_D5, YM_D6, YM_D7};

int registers[0x38] = {0};
int register_changes[REGISTER_CHANGES_MAX];
int register_changes_sum = 0;

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

void add_register_change(int address) {
  register_changes[register_changes_sum] = address;
  register_changes_sum++;
}

void store_register_byte(int address, int data) {
  registers[address] = data;
  add_register_change(address);
}

void toggle_register_bit(int address, int bit) {
  registers[address] ^= 1 << bit;
  add_register_change(address);
}

void store_register_range(int address, int data, int position, int length) {
  int mask = ((1 << length) - 1) << position;
  registers[address] = (registers[address] & ~mask) | (data << position & mask);
  add_register_change(address);
}

void setup_ym2420() {
  for (int pin = 0; pin < 8; pin++) {
    pinMode(YM_D[pin], OUTPUT);
  }
  pinMode(YM_A0, OUTPUT);
  pinMode(YM_CS, OUTPUT);
  pinMode(YM_IC, OUTPUT);

  digitalWrite(YM_A0, 1);
  digitalWrite(YM_CS, 1);
  digitalWrite(YM_IC, 1);

  reset_ym2420();
}

void reset_ym2420() {
  digitalWrite(YM_IC, 0);
  delayMicroseconds(RESET_TIME);
  digitalWrite(YM_IC, 1);
}

void apply_byte(int data) {
  for (int pin = 0; pin < 8; pin++) {
    digitalWrite(YM_D[pin], (data >> pin) % 2);
  }
}

void write_ym2420_changes() {
  for (int change = 0; change < register_changes_sum; change++) {
    apply_byte(register_changes[change]);
    digitalWrite(YM_A0, 0);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);

    apply_byte(registers[register_changes[change]]);
    digitalWrite(YM_A0, 1);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);
  }

  register_changes_sum = 0;
}

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

  octave(oscillator, octave_number + 2);
  f_number(oscillator, f_number_int);
}
