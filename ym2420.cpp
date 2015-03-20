#include "ym2420.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define YM2420_DATA_MODE true
#define YM2420_ADDRESS_MODE false

#define YM2420_CS 3

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

uint8_t register_contents[0x38];

void ym2420_setup() {
  pinMode(YM2420_CS, OUTPUT);
  digitalWrite(YM2420_CS, HIGH);

  for (uint8_t oscillator = 0; oscillator < YM2420_OSCILLATORS; oscillator++) {
    instrument.set(oscillator, INSTRUMENT_ORIGINAL);
  }

  sustained_sound_carrier.set(true);
  sustained_sound_modulation.set(true);
}


void write(uint8_t address) {
  mcp23s17_write_parallel(YM2420_CS, YM2420_ADDRESS_MODE, address);
  mcp23s17_write_parallel(
      YM2420_CS, YM2420_DATA_MODE, register_contents[address]);
}

void write_bit(uint8_t address, uint8_t bit, bool value) {
  register_contents[address] ^= (
      -value ^ register_contents[address]) & (1 << bit);
  write(address);
}

void write_range(
    uint8_t address, uint8_t first_bit, uint8_t range, bool inverted,
    uint8_t value) {
  if (inverted) {
    value = range - value;
  }

  uint8_t mask = range << first_bit;
  register_contents[address] = (
      register_contents[address] & ~mask) | (value << first_bit & mask);
  write(address);
}


YM2420Bit::YM2420Bit(uint8_t a, uint8_t b):
  address(a), bit_number(b) {}

void YM2420Bit::set(uint8_t value) {
  write_bit(address, bit_number, value);
}


YM2420Range::YM2420Range(uint8_t a, uint8_t b, uint8_t c, bool d):
  address(a), first_bit(b), range(c), inverted(d) {}

uint8_t YM2420Range::get_range() {
  return range;
}

void YM2420Range::set(uint8_t value) {
  write_range(address, first_bit, range, inverted, value);
}


YM2420OscillatorBit::YM2420OscillatorBit(uint8_t a, uint8_t b):
  address(a), bit_number(b) {}

void YM2420OscillatorBit::set(uint8_t oscillator, uint8_t value) {
  write_bit(address + oscillator, bit_number, value);
}


YM2420OscillatorRange::YM2420OscillatorRange(
    uint8_t a, uint8_t b, uint8_t c, bool d):
  address(a), first_bit(b), range(c), inverted(d) {}

uint8_t YM2420OscillatorRange::get_range() {
  return range;
}

void YM2420OscillatorRange::set(uint8_t oscillator, uint8_t value) {
  write_range(address + oscillator, first_bit, range, inverted, value);
}


YM2420OscillatorFrequency::YM2420OscillatorFrequency(
    YM2420OscillatorRange* a, uint8_t b, uint8_t c, uint8_t d, uint8_t e,
    uint8_t f, uint8_t g):
  octave(a), address_lsb(b), first_bit_lsb(c), range_lsb(d),
  address_msb(e), first_bit_msb(f), range_msb(g) {
    for (uint8_t key = 0; key < YM2420_F_NUMBER_KEYS; key++) {
      f_numbers[key] = pow(2, (float)(key - 48) / 12) * 288.3584;
    }
}

void YM2420OscillatorFrequency::set_f_number(
    uint8_t oscillator, unsigned int f_number) {
  write_range(
      address_lsb + oscillator, first_bit_lsb, range_lsb, false,
      f_number >> 4);
  write_range(
      address_msb + oscillator, first_bit_msb, range_msb, false,
      f_number & 0b1111);
}

void YM2420OscillatorFrequency::set_key(
    uint8_t oscillator, uint8_t key_number) {
  uint8_t octave_number = ((int)f_numbers[key_number] / 512) + 2;
  unsigned int f_number = f_numbers[key_number] / pow(2, octave_number);

  octave->set(oscillator, octave_number);
  set_f_number(oscillator, f_number);
}


YM2420Bit amplitude_modulation_carrier    (0x00, 7);
YM2420Bit amplitude_modulation_modulation (0x01, 7);
YM2420Bit vibrato_carrier                 (0x00, 6);
YM2420Bit vibrato_modulation              (0x01, 6);
YM2420Bit sustained_sound_carrier         (0x00, 5);
YM2420Bit sustained_sound_modulation      (0x01, 5);
YM2420Bit rate_key_scale_carrier          (0x00, 4);
YM2420Bit rate_key_scale_modulation       (0x01, 4);
YM2420Bit wave_distortion_carrier         (0x03, 3);
YM2420Bit wave_distortion_modulation      (0x03, 4);
YM2420Bit rhythm_sound                    (0x0e, 5);

YM2420Range multi_sample_wave_carrier    (0x00, 0, 15, false);
YM2420Range multi_sample_wave_modulation (0x01, 0, 15, false);
YM2420Range level_key_scale_carrier      (0x02, 6, 3,  true);
YM2420Range level_key_scale_modulation   (0x03, 6, 3,  true);
YM2420Range modulation_index             (0x02, 0, 63, true);
YM2420Range fm_feedback_constant         (0x03, 0, 7,  false);
YM2420Range attack_rate_carrier          (0x04, 4, 15, true);
YM2420Range decay_rate_carrier           (0x04, 0, 15, true);
YM2420Range sustain_rate_carrier         (0x06, 4, 15, true);
YM2420Range release_rate_carrier         (0x06, 0, 15, true);
YM2420Range attack_rate_modulation       (0x05, 4, 15, true);
YM2420Range decay_rate_modulation        (0x05, 0, 15, true);
YM2420Range sustain_rate_modulation      (0x07, 4, 15, true);
YM2420Range release_rate_modulation      (0x07, 0, 15, true);
YM2420Range rhythm_instruments           (0x0e, 0, 31, false);

YM2420OscillatorBit sustain (0x20, 5);
YM2420OscillatorBit key     (0x20, 4);

YM2420OscillatorRange  octave     (0x10, 5, 7,  false);
YM2420OscillatorRange  instrument (0x30, 4, 15, false);
YM2420OscillatorRange  volume     (0x30, 0, 15, true);

YM2420OscillatorFrequency frequency (&octave, 0x10, 0, 31, 0x20, 0, 15);
