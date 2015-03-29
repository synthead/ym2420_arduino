#include "mcp3008.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "hd44780.h"
#include "menu.h"
#include <Arduino.h>

#define MCP3008_0_CS 6
#define MCP3008_1_CS 7

#define ANALOG_CONTROL_COUNT 14
#define DIGITAL_CONTROL_COUNT 8

namespace Controls {
  struct analog_control {
    const uint8_t chip_select;
    const uint8_t pin;
    YM2420::Range* ym2420_range;
    const char* line1;
    const char* line2;
    uint8_t value;
  };

  struct digital_control {
    const uint8_t pin;
    YM2420::Bit* ym2420_bit;
    const char* line1;
    const char* line2;
    bool value;
  };

  analog_control analog_controls[ANALOG_CONTROL_COUNT] = {
      {.chip_select = MCP3008_0_CS,
       .pin = 0,
       .ym2420_range = &YM2420::multi_sample_wave_carrier,
       .line1 = "Multi-smple wave",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_0_CS,
       .pin = 1,
       .ym2420_range = &YM2420::multi_sample_wave_modulation,
       .line1 = "Multi-smple wave",
       .line2 = "Modulation",
       .value = -1},

      {.chip_select = MCP3008_0_CS,
       .pin = 2,
       .ym2420_range = &YM2420::modulation_index,
       .line1 = "Modulation index",
       .line2 = "",
       .value = -1},

      {.chip_select = MCP3008_0_CS,
       .pin = 3,
       .ym2420_range = &YM2420::fm_feedback_constant,
       .line1 = "FM feedback",
       .line2 = "",
       .value = -1},

      {.chip_select = MCP3008_0_CS,
       .pin = 4,
       .ym2420_range = &YM2420::level_key_scale_carrier,
       .line1 = "LEVEL key scale",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_0_CS,
       .pin = 5,
       .ym2420_range = &YM2420::level_key_scale_modulation,
       .line1 = "LEVEL key scale",
       .line2 = "Modulation",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 0,
       .ym2420_range = &YM2420::attack_rate_carrier,
       .line1 = "Attack",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 1,
       .ym2420_range = &YM2420::decay_rate_carrier,
       .line1 = "Decay",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 2,
       .ym2420_range = &YM2420::sustain_rate_carrier,
       .line1 = "Sustain",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 3,
       .ym2420_range = &YM2420::release_rate_carrier,
       .line1 = "Release",
       .line2 = "Carrier",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 4,
       .ym2420_range = &YM2420::attack_rate_modulation,
       .line1 = "Attack",
       .line2 = "Modulation",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 5,
       .ym2420_range = &YM2420::decay_rate_modulation,
       .line1 = "Decay",
       .line2 = "Modulation",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 6,
       .ym2420_range = &YM2420::sustain_rate_modulation,
       .line1 = "Sustain",
       .line2 = "Modulation",
       .value = -1},

      {.chip_select = MCP3008_1_CS,
       .pin = 7,
       .ym2420_range = &YM2420::release_rate_modulation,
       .line1 = "Release",
       .line2 = "Modulation",
       .value = -1}
  };

  digital_control digital_controls[DIGITAL_CONTROL_COUNT] = {
      {.pin = 0,
       .ym2420_bit = &YM2420::amplitude_modulation_carrier,
       .line1 = "Amplitude Modltn",
       .line2 = "Carrier"},

      {.pin = 1,
       .ym2420_bit = &YM2420::vibrato_carrier,
       .line1 = "Vibrato",
       .line2 = "Carrier"},

      {.pin = 2,
       .ym2420_bit = &YM2420::wave_distortion_carrier,
       .line1 = "Wave distortion",
       .line2 = "Carrier"},

      {.pin = 3,
       .ym2420_bit = &YM2420::rate_key_scale_carrier,
       .line1 = "RATE key scale",
       .line2 = "Carrier"},

      {.pin = 4,
       .ym2420_bit = &YM2420::amplitude_modulation_modulation,
       .line1 = "Amplitude Modltn",
       .line2 = "Modulation"},

      {.pin = 5,
       .ym2420_bit = &YM2420::vibrato_modulation,
       .line1 = "Vibrato",
       .line2 = "Modulation"},

      {.pin = 6,
       .ym2420_bit = &YM2420::wave_distortion_modulation,
       .line1 = "Wave distortion",
       .line2 = "Modulation"},

      {.pin = 7,
       .ym2420_bit = &YM2420::rate_key_scale_modulation,
       .line1 = "RATE key scale",
       .line2 = "Modulation"}
  };

  void print_analog_control(analog_control control) {
    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    HD44780::position_print(0, 0, line1_padded);

    char line2_padded[13];
    sprintf(line2_padded, "%-12s", control.line2);
    HD44780::position_print(0, 1, line2_padded);

    char percent_text[5];
    sprintf(
        percent_text, "%3d%%",
        control.value * 100 / control.ym2420_range->get_range());
    HD44780::print(percent_text);

    Menu::set_temporary_message();
  }

  void print_digital_control(digital_control control) {
    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    HD44780::position_print(0, 0, line1_padded);

    char line2_padded[14];
    sprintf(line2_padded, "%-13s", control.line2);
    HD44780::position_print(0, 1, line2_padded);

    HD44780::print(control.value ? "On " : "Off");

    Menu::set_temporary_message();
  }

  void apply_analog(bool print_to_lcd) {
    for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
      unsigned int value = MCP3008::read(
          analog_controls[control].chip_select,
          analog_controls[control].pin,
          analog_controls[control].ym2420_range->get_range());

      if (value != analog_controls[control].value) {
        analog_controls[control].value = value;
        analog_controls[control].ym2420_range->set(value);

        if (print_to_lcd) {
          print_analog_control(analog_controls[control]);
        }
      }
    }
  }

  void apply_digital(bool print_to_lcd) {
    uint8_t values = MCP23S17::scan_matrix(6);

    for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
      bool value = (values >> digital_controls[control].pin) & 1;

      if (value != digital_controls[control].value) {
        digital_controls[control].value = value;
        digital_controls[control].ym2420_bit->set(value);

        if (print_to_lcd) {
          print_digital_control(digital_controls[control]);
        }
      }
    }
  }

  void setup() {
    MCP3008::setup(MCP3008_0_CS);
    MCP3008::setup(MCP3008_1_CS);

    apply_analog(false);
    apply_digital(false);
  }

  void apply() {
    apply_analog(! Menu::temporary_message.menu_active);
    apply_digital(! Menu::temporary_message.menu_active);
  }
}
