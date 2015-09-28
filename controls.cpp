#include "controls.h"
#include "mcp3008.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "hd44780.h"
#include "menu.h"
#include "midi.h"
#include <Arduino.h>

#define DIGITAL_CONTROLS_COLUMN 6

#define MCP3008_0_CS 6
#define MCP3008_1_CS 7

namespace Controls {
  uint8_t digital_values = 0;

  void update_digital_values() {
    digital_values = MCP23S17::scan_matrix(DIGITAL_CONTROLS_COLUMN);
  }

  AnalogControl::AnalogControl(
      const uint8_t a, const uint8_t b, uint8_t* c, YM2420::Range* d,
      const char* e, const char* f):
    chip_select(a), pin(b), cc_number(c), ym2420_range(d), line_1(e), line_2(f)
    {}

  uint8_t AnalogControl::get_midi_value() {
    return MCP3008::read_midi_value(chip_select, pin);
  }

  uint8_t AnalogControl::get_param_value() {
    return midi_value * ym2420_range->get_range() / MIDI_DATA_BYTE_MAX;
  }

  bool AnalogControl::set_via_cc_number(
      uint8_t check_cc_number, uint8_t midi_value) {
    if (*cc_number == check_cc_number) {
      ym2420_range->set(
          midi_value * ym2420_range->get_range() / MIDI_DATA_BYTE_MAX);
      return true;
    } else {
      return false;
    }
  }

  void AnalogControl::set_current_value() {
    midi_value = get_midi_value();
    param_value = get_param_value();

    ym2420_range->set(param_value);
  }

  void AnalogControl::check_value() {
    uint8_t new_midi_value = get_midi_value();

    if (new_midi_value != midi_value) {
      midi_value = get_midi_value();
      MIDI::send(MIDI_CC, *cc_number, midi_value);

      uint8_t new_param_value = get_param_value();

      if (new_param_value != param_value) {
        param_value = new_param_value;
        ym2420_range->set(param_value);

        if (! Menu::active_menu) {
          HD44780::position_print(0, 0, line_1);
          HD44780::position_print(0, 1, line_2);

          char percent_text[7];
          sprintf(
              percent_text, "  %3d%%",
              param_value * 100 / ym2420_range->get_range());
          HD44780::print(percent_text);

          Menu::set_expiration();
        }
      }
    }
  }

  DigitalControl::DigitalControl(
      const uint8_t a, uint8_t* b, YM2420::Bit* c, const char* d,
      const char* e):
    pin(a), cc_number(b), ym2420_bit(c), line_1(d), line_2(e) {}

  bool DigitalControl::read() {
    return (digital_values >> pin) & 1;
  }

  void DigitalControl::set_current_value() {
    value = read();
    ym2420_bit->set(value);
  }

  bool DigitalControl::set_via_cc_number(
      uint8_t check_cc_number, uint8_t midi_value) {
    if (*cc_number == check_cc_number) {
      ym2420_bit->set(midi_value >= 64);
      return true;
    } else {
      return false;
    }
  }

  void DigitalControl::check_value() {
    bool new_value = (digital_values >> pin) & 1;

    if (new_value != value) {
      value = new_value;
      ym2420_bit->set(new_value);
      MIDI::send(
          MIDI_CC, *cc_number,
          value ? MIDI_DATA_BYTE_MAX : MIDI_DATA_BYTE_MIN);

      if (! Menu::active_menu) {
        HD44780::position_print(0, 0, line_1);
        HD44780::position_print(0, 1, line_2);
        HD44780::print(value ? "On " : "Off");

        Menu::set_expiration();
      }
    }
  }

  AnalogControl analog_controls[ANALOG_CONTROL_COUNT] = {
      {MCP3008_0_CS, 0,
       &MIDI::CC::multi_sample_wave_carrier,
       &YM2420::multi_sample_wave_carrier,
       "Multi-smple wave", "Carrier   "},

      {MCP3008_0_CS, 1,
       &MIDI::CC::multi_sample_wave_modulation,
       &YM2420::multi_sample_wave_modulation,
       "Multi-smple wave", "Modulation"},

      {MCP3008_0_CS, 2,
       &MIDI::CC::modulation_index,
       &YM2420::modulation_index,
       "Modulation index", "          "},

      {MCP3008_0_CS, 3,
       &MIDI::CC::fm_feedback_constant,
       &YM2420::fm_feedback_constant,
       "FM feedback     ", "          "},

      {MCP3008_0_CS, 4,
       &MIDI::CC::level_key_scale_carrier,
       &YM2420::level_key_scale_carrier,
       "LEVEL key scale ", "Carrier   "},

      {MCP3008_0_CS, 5,
       &MIDI::CC::level_key_scale_modulation,
       &YM2420::level_key_scale_modulation,
       "LEVEL key scale ", "Modulation"},

      {MCP3008_1_CS, 0,
       &MIDI::CC::attack_rate_carrier,
       &YM2420::attack_rate_carrier,
       "Attack          ", "Carrier   "},

      {MCP3008_1_CS, 1,
       &MIDI::CC::decay_rate_carrier,
       &YM2420::decay_rate_carrier,
       "Decay           ", "Carrier   "},

      {MCP3008_1_CS, 2,
       &MIDI::CC::sustain_rate_carrier,
       &YM2420::sustain_rate_carrier,
       "Sustain         ", "Carrier   "},

      {MCP3008_1_CS, 3,
       &MIDI::CC::release_rate_carrier,
       &YM2420::release_rate_carrier,
       "Release         ", "Carrier   "},

      {MCP3008_1_CS, 4,
       &MIDI::CC::attack_rate_modulation,
       &YM2420::attack_rate_modulation,
       "Attack          ", "Modulation"},

      {MCP3008_1_CS, 5,
       &MIDI::CC::decay_rate_modulation,
       &YM2420::decay_rate_modulation,
       "Decay           ", "Modulation"},

      {MCP3008_1_CS, 6,
       &MIDI::CC::sustain_rate_modulation,
       &YM2420::sustain_rate_modulation,
       "Sustain         ", "Modulation"},

      {MCP3008_1_CS, 7,
       &MIDI::CC::release_rate_modulation,
       &YM2420::release_rate_modulation,
       "Release         ", "Modulation"}};

  DigitalControl digital_controls[DIGITAL_CONTROL_COUNT] = {
      {0, &MIDI::CC::amplitude_modulation_carrier,
       &YM2420::amplitude_modulation_carrier,
       "Amplitude Modltn", "Carrier   "},

      {1, &MIDI::CC::vibrato_carrier,
       &YM2420::vibrato_carrier,
       "Vibrato         ", "Carrier   "},

      {2, &MIDI::CC::wave_distortion_carrier,
       &YM2420::wave_distortion_carrier,
       "Wave distortion ", "Carrier   "},

      {3, &MIDI::CC::rate_key_scale_carrier,
       &YM2420::rate_key_scale_carrier,
       "RATE key scale  ", "Carrier   "},

      {4, &MIDI::CC::amplitude_modulation_modulation,
       &YM2420::amplitude_modulation_modulation,
       "Amplitude Modltn", "Modulation"},

      {5, &MIDI::CC::vibrato_modulation,
       &YM2420::vibrato_modulation,
       "Vibrato         ", "Modulation"},

      {6, &MIDI::CC::wave_distortion_modulation,
       &YM2420::wave_distortion_modulation,
       "Wave distortion ", "Modulation"},

      {7, &MIDI::CC::rate_key_scale_modulation,
       &YM2420::rate_key_scale_modulation,
       "RATE key scale  ", "Modulation"}};

  void set_current_values() {
    update_digital_values();

    for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
      analog_controls[control].set_current_value();
    }

    for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
      digital_controls[control].set_current_value();
    }
  }

  void check_values() {
    update_digital_values();

    for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
      analog_controls[control].check_value();
    }

    for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
      digital_controls[control].check_value();
    }
  }

  void setup() {
    MCP3008::setup(MCP3008_0_CS);
    MCP3008::setup(MCP3008_1_CS);

    set_current_values();
  }
}
