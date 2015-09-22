#include "ym2420.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define YM2420_DATA_MODE true
#define YM2420_ADDRESS_MODE false

#define YM2420_CS 3
#define YM2420_IC 10

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

namespace YM2420 {
  uint8_t register_contents[YM2420_REGISTERS] = {0};
  uint8_t active_oscillators[YM2420_OSCILLATORS] = {false};
  uint8_t last_oscillator = 0;

  void setup() {
    pinMode(YM2420_CS, OUTPUT);
    digitalWrite(YM2420_CS, HIGH);

    pinMode(YM2420_IC, OUTPUT);
    digitalWrite(YM2420_IC, HIGH);

    for (uint8_t oscillator = 0; oscillator < YM2420_OSCILLATORS;
         oscillator++) {
      instrument.set(oscillator, INSTRUMENT_ORIGINAL);
      octave.set(oscillator, 3);
    }

    sustained_sound_carrier.set(true);
    sustained_sound_modulation.set(true);
  }

  void write(uint8_t address) {
    MCP23S17::write_parallel(YM2420_CS, YM2420_ADDRESS_MODE, address);
    MCP23S17::write_parallel(
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


  Bit::Bit(uint8_t a, uint8_t b):
    address(a), bit_number(b) {}

  void Bit::set(uint8_t value) {
    write_bit(address, bit_number, value);
  }


  Range::Range(uint8_t a, uint8_t b, uint8_t c, bool d):
    address(a), first_bit(b), range(c), inverted(d) {}

  uint8_t Range::get_range() {
    return range;
  }

  void Range::set(uint8_t value) {
    write_range(address, first_bit, range, inverted, value);
  }


  OscillatorBit::OscillatorBit(uint8_t a, uint8_t b):
    address(a), bit_number(b) {}

  void OscillatorBit::set(uint8_t oscillator, uint8_t value) {
    write_bit(address + oscillator, bit_number, value);
  }


  OscillatorRange::OscillatorRange(
      uint8_t a, uint8_t b, uint8_t c, bool d):
    address(a), first_bit(b), range(c), inverted(d) {}

  uint8_t OscillatorRange::get_range() {
    return range;
  }

  void OscillatorRange::set(uint8_t oscillator, uint8_t value) {
    write_range(address + oscillator, first_bit, range, inverted, value);
  }

  Bit amplitude_modulation_carrier    (0x00, 7);
  Bit amplitude_modulation_modulation (0x01, 7);
  Bit vibrato_carrier                 (0x00, 6);
  Bit vibrato_modulation              (0x01, 6);
  Bit sustained_sound_carrier         (0x00, 5);
  Bit sustained_sound_modulation      (0x01, 5);
  Bit rate_key_scale_carrier          (0x00, 4);
  Bit rate_key_scale_modulation       (0x01, 4);
  Bit wave_distortion_carrier         (0x03, 3);
  Bit wave_distortion_modulation      (0x03, 4);
  Bit rhythm_sound                    (0x0e, 5);

  Range multi_sample_wave_carrier    (0x00, 0, 15, false);
  Range multi_sample_wave_modulation (0x01, 0, 15, false);
  Range level_key_scale_carrier      (0x02, 6, 3,  true);
  Range level_key_scale_modulation   (0x03, 6, 3,  true);
  Range modulation_index             (0x02, 0, 63, true);
  Range fm_feedback_constant         (0x03, 0, 7,  false);
  Range attack_rate_carrier          (0x04, 4, 15, true);
  Range decay_rate_carrier           (0x04, 0, 15, true);
  Range sustain_rate_carrier         (0x06, 4, 15, true);
  Range release_rate_carrier         (0x06, 0, 15, true);
  Range attack_rate_modulation       (0x05, 4, 15, true);
  Range decay_rate_modulation        (0x05, 0, 15, true);
  Range sustain_rate_modulation      (0x07, 4, 15, true);
  Range release_rate_modulation      (0x07, 0, 15, true);
  Range rhythm_instruments           (0x0e, 0, 31, false);

  OscillatorBit sustain  (0x20, 5);
  OscillatorBit key_down (0x20, 4);

  OscillatorRange octave       (0x10, 5, 7,  false);
  OscillatorRange instrument   (0x30, 4, 15, false);
  OscillatorRange volume       (0x30, 0, 15, true);
  OscillatorRange f_number_lsb (0x20, 0, 15, false);
  OscillatorRange f_number_msb (0x10, 0, 31, false);

  uint8_t oscillator_on(uint8_t key, uint8_t velocity) {
    for (uint8_t oscillator_check = 0; oscillator_check < YM2420_OSCILLATORS;
         oscillator_check++) {
      uint8_t oscillator = (
          oscillator_check + last_oscillator + 1) % YM2420_OSCILLATORS;

      if (! active_oscillators[oscillator]) {
        uint16_t f_number = roundf(pow(2, (float)(key - 57) / 12) * 288.3584);

        f_number_lsb.set(oscillator, f_number & 0b1111);
        f_number_msb.set(oscillator, f_number >> 4);
        volume.set(oscillator, velocity);
        key_down.set(oscillator, true);

        active_oscillators[oscillator] = true;
        last_oscillator = oscillator;

        return oscillator;
      }
    }

    return YM2420_NO_OSCILLATOR;
  }

  void oscillator_off(uint8_t oscillator) {
    key_down.set(oscillator, false);
    active_oscillators[oscillator] = false;
  }

  void panic() {
    digitalWrite(YM2420_IC, LOW);

    for (uint8_t oscillator = 0; oscillator < YM2420_OSCILLATORS;
         oscillator++) {
      active_oscillators[oscillator] = false;
    }

    setup();

    for (uint8_t address = 0x00; address < YM2420_PATCH_BYTES; address++) {
      write(address);
    }
  }
}
