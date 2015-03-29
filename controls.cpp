#include "controls.h"
#include "mcp3008.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "hd44780.h"
#include "menu.h"
#include <Arduino.h>

#define DIGITAL_CONTROLS_COLUMN 6

#define MCP3008_0_CS 6
#define MCP3008_1_CS 7

#define ANALOG_CONTROL_COUNT 14
#define DIGITAL_CONTROL_COUNT 8

namespace Controls {
  uint8_t digital_values = 0;

  void update_digital_values() {
    digital_values = MCP23S17::scan_matrix(DIGITAL_CONTROLS_COLUMN);
  }

  AnalogControl::AnalogControl(
      const uint8_t a, const uint8_t b, YM2420::Range* c, const char* d,
      const char* e):
    chip_select(a), pin(b), ym2420_range(c), line1(d), line2(e) {}

  bool AnalogControl::scan() {
    unsigned int new_value = MCP3008::read(
        chip_select, pin, ym2420_range->get_range());

    if (new_value != value) {
      value = new_value;
      ym2420_range->set(new_value);
      return true;
    } else {
      return false;
    }
  }

  void AnalogControl::print() {
    HD44780::position_print(0, 0, line1);
    HD44780::position_print(0, 1, line2);

    char percent_text[5];
    sprintf(percent_text, "%3d%%", value * 100 / ym2420_range->get_range());
    HD44780::print(percent_text);

    Menu::set_temporary_message();
  }

  void AnalogControl::update() {
    if(scan() && ! Menu::temporary_message.menu_active) {
      print();
    }
  }

  DigitalControl::DigitalControl(
      const uint8_t a, YM2420::Bit* b, const char* c, const char* d):
    pin(a), ym2420_bit(b), line1(c), line2(d) {}

  bool DigitalControl::scan() {
    bool new_value = (digital_values >> pin) & 1;

    if (new_value != value) {
      value = new_value;
      ym2420_bit->set(new_value);
      return true;
    } else {
      return false;
    }
  }

  void DigitalControl::print() {
    HD44780::position_print(0, 0, line1);
    HD44780::position_print(0, 1, line2);
    HD44780::print(value ? "On " : "Off");

    Menu::set_temporary_message();
  }

  void DigitalControl::update() {
    if(scan() && ! Menu::temporary_message.menu_active) {
      print();
    }
  }

  AnalogControl analog_controls[ANALOG_CONTROL_COUNT] = {
      {MCP3008_0_CS, 0,
       &YM2420::multi_sample_wave_carrier,
       "Multi-smple wave", "Carrier     "},

      {MCP3008_0_CS, 1,
       &YM2420::multi_sample_wave_modulation,
       "Multi-smple wave", "Modulation  "},

      {MCP3008_0_CS, 2,
       &YM2420::modulation_index,
       "Modulation index", "            "},

      {MCP3008_0_CS, 3,
       &YM2420::fm_feedback_constant,
       "FM feedback     ", "            "},

      {MCP3008_0_CS, 4,
       &YM2420::level_key_scale_carrier,
       "LEVEL key scale ", "Carrier     "},

      {MCP3008_0_CS, 5,
       &YM2420::level_key_scale_modulation,
       "LEVEL key scale ", "Modulation  "},

      {MCP3008_1_CS, 0,
       &YM2420::attack_rate_carrier,
       "Attack          ", "Carrier     "},

      {MCP3008_1_CS, 1,
       &YM2420::decay_rate_carrier,
       "Decay           ", "Carrier     "},

      {MCP3008_1_CS, 2,
       &YM2420::sustain_rate_carrier,
       "Sustain         ", "Carrier     "},

      {MCP3008_1_CS, 3,
       &YM2420::release_rate_carrier,
       "Release         ", "Carrier     "},

      {MCP3008_1_CS, 4,
       &YM2420::attack_rate_modulation,
       "Attack          ", "Modulation  "},

      {MCP3008_1_CS, 5,
       &YM2420::decay_rate_modulation,
       "Decay           ", "Modulation  "},

      {MCP3008_1_CS, 6,
       &YM2420::sustain_rate_modulation,
       "Sustain         ", "Modulation  "},

      {MCP3008_1_CS, 7,
       &YM2420::release_rate_modulation,
       "Release         ", "Modulation  "}};


  DigitalControl digital_controls[DIGITAL_CONTROL_COUNT] = {
      {0, &YM2420::amplitude_modulation_carrier,
       "Amplitude Modltn", "Carrier      "},

      {1, &YM2420::vibrato_carrier,
       "Vibrato         ", "Carrier      "},

      {2, &YM2420::wave_distortion_carrier,
       "Wave distortion ", "Carrier      "},

      {3, &YM2420::rate_key_scale_carrier,
       "RATE key scale  ", "Carrier      "},

      {4, &YM2420::amplitude_modulation_modulation,
       "Amplitude Modltn", "Modulation   "},

      {5, &YM2420::vibrato_modulation,
       "Vibrato         ", "Modulation   "},

      {6, &YM2420::wave_distortion_modulation,
       "Wave distortion ", "Modulation   "},

      {7, &YM2420::rate_key_scale_modulation,
       "RATE key scale  ", "Modulation   "}};

  void setup() {
    MCP3008::setup(MCP3008_0_CS);
    MCP3008::setup(MCP3008_1_CS);

    update_digital_values();

    for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
      analog_controls[control].scan();
    }

    for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
      digital_controls[control].scan();
    }
  }

  void update_all() {
    update_digital_values();

    for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
      analog_controls[control].update();
    }

    for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
      digital_controls[control].update();
    }
  }
}
