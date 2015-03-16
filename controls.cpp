#include "mcp3008.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include "hd44780.h"
#include <Arduino.h>

#define MCP3008_0_CS 6
#define MCP3008_1_CS 7
#define MCP23S17_HW_ADDRESS 0b000

#define ANALOG_CONTROL_COUNT 14
#define DIGITAL_CONTROL_COUNT 8

struct analog_change {
  uint8_t chip_select;
  uint8_t pin;
};

struct analog_control {
  const uint8_t chip_select;
  const uint8_t pin;
  ym2420_range_t ym2420_range;
  const char* line1;
  const char* line2;
  uint8_t value;
};

struct digital_control {
  const uint8_t pin;
  ym2420_bit_t ym2420_bit;
  const char* line1;
  const char* line2;
  bool value;
};

analog_change last_analog_change = {
  .chip_select = -1,
  .pin = -1
};

uint8_t last_digital_change_pin = -1;

analog_control analog_controls[ANALOG_CONTROL_COUNT] = {
    {.chip_select = MCP3008_0_CS,
     .pin = 0,
     .ym2420_range = ym2420_ranges.multi_sample_wave_carrier,
     .line1 = "Multi-smple wave",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_0_CS,
     .pin = 1,
     .ym2420_range = ym2420_ranges.multi_sample_wave_modulation,
     .line1 = "Multi-smple wave",
     .line2 = "Modulation"},

    {.chip_select = MCP3008_0_CS,
     .pin = 2,
     .ym2420_range = ym2420_ranges.modulation_index,
     .line1 = "Modulation index",
     .line2 = ""},

    {.chip_select = MCP3008_0_CS,
     .pin = 3,
     .ym2420_range = ym2420_ranges.fm_feedback_constant,
     .line1 = "FM feedback",
     .line2 = ""},

    {.chip_select = MCP3008_0_CS,
     .pin = 4,
     .ym2420_range = ym2420_ranges.level_key_scale_carrier,
     .line1 = "LEVEL key scale",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_0_CS,
     .pin = 5,
     .ym2420_range = ym2420_ranges.level_key_scale_modulation,
     .line1 = "LEVEL key scale",
     .line2 = "Modulation"},

    {.chip_select = MCP3008_1_CS,
     .pin = 0,
     .ym2420_range = ym2420_ranges.attack_rate_carrier,
     .line1 = "Attack",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_1_CS,
     .pin = 1,
     .ym2420_range = ym2420_ranges.decay_rate_carrier,
     .line1 = "Decay",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_1_CS,
     .pin = 2,
     .ym2420_range = ym2420_ranges.sustain_rate_carrier,
     .line1 = "Sustain",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_1_CS,
     .pin = 3,
     .ym2420_range = ym2420_ranges.release_rate_carrier,
     .line1 = "Release",
     .line2 = "Carrier"},

    {.chip_select = MCP3008_1_CS,
     .pin = 4,
     .ym2420_range = ym2420_ranges.attack_rate_modulation,
     .line1 = "Attack",
     .line2 = "Modulation"},

    {.chip_select = MCP3008_1_CS,
     .pin = 5,
     .ym2420_range = ym2420_ranges.decay_rate_modulation,
     .line1 = "Decay",
     .line2 = "Modulation"},

    {.chip_select = MCP3008_1_CS,
     .pin = 6,
     .ym2420_range = ym2420_ranges.sustain_rate_modulation,
     .line1 = "Sustain",
     .line2 = "Modulation"},

    {.chip_select = MCP3008_1_CS,
     .pin = 7,
     .ym2420_range = ym2420_ranges.release_rate_modulation,
     .line1 = "Release",
     .line2 = "Modulation"}
};

digital_control digital_controls[DIGITAL_CONTROL_COUNT] = {
    {.pin = 0,
     .ym2420_bit = ym2420_bits.amplitude_modulation_carrier,
     .line1 = "Amplitude Modltn",
     .line2 = "Carrier"},

    {.pin = 1,
     .ym2420_bit = ym2420_bits.vibrato_carrier,
     .line1 = "Vibrato",
     .line2 = "Carrier"},

    {.pin = 2,
     .ym2420_bit = ym2420_bits.wave_distortion_carrier,
     .line1 = "Wave distortion",
     .line2 = "Carrier"},

    {.pin = 3,
     .ym2420_bit = ym2420_bits.rate_key_scale_carrier,
     .line1 = "RATE key scale",
     .line2 = "Carrier"},

    {.pin = 4,
     .ym2420_bit = ym2420_bits.amplitude_modulation_modulation,
     .line1 = "Amplitude Modltn",
     .line2 = "Modulation"},

    {.pin = 5,
     .ym2420_bit = ym2420_bits.vibrato_modulation,
     .line1 = "Vibrato",
     .line2 = "Modulation"},

    {.pin = 6,
     .ym2420_bit = ym2420_bits.wave_distortion_modulation,
     .line1 = "Wave distortion",
     .line2 = "Modulation"},

    {.pin = 7,
     .ym2420_bit = ym2420_bits.rate_key_scale_modulation,
     .line1 = "RATE key scale",
     .line2 = "Modulation"}
};

void hd44780_print_analog_control(analog_control control) {
  char percent_text[5];
  sprintf(percent_text, "%3d%%", control.value * 100 / control.ym2420_range.range);

  if (last_analog_change.chip_select != control.chip_select ||
      last_analog_change.pin != control.pin) {
    last_analog_change.chip_select = control.chip_select;
    last_analog_change.pin = control.pin;

    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    hd44780_print_position(0, 0, line1_padded);

    char line2_padded[13];
    sprintf(line2_padded, "%-12s", control.line2);
    hd44780_print_position(0, 1, line2_padded);

    hd44780_print(percent_text);
  } else {
    hd44780_print_position(12, 1, percent_text);
  }
}

void hd44780_print_digital_control(digital_control control) {
  const char* state = control.value ? "On " : "Off";

  if (last_digital_change_pin != control.pin) {
    last_digital_change_pin = control.pin;

    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    hd44780_print_position(0, 0, line1_padded);

    char line2_padded[14];
    sprintf(line2_padded, "%-13s", control.line2);
    hd44780_print_position(0, 1, line2_padded);

    hd44780_print(state);
  } else {
    hd44780_print_position(13, 1, state);
  }
}

void apply_analog_controls(bool print_to_lcd) {
  for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
    unsigned int value = mcp3008_read(
        analog_controls[control].chip_select,
        analog_controls[control].pin,
        analog_controls[control].ym2420_range.range);

    if (value != analog_controls[control].value) {
      analog_controls[control].value = value;
      ym2420_write_range(analog_controls[control].ym2420_range, value);

      if (print_to_lcd) {
        hd44780_print_analog_control(analog_controls[control]);
      }
    }
  }
}

void apply_digital_controls(bool print_to_lcd) {
  uint8_t values = mcp23s17_read(MCP23S17_HW_ADDRESS, MCP23S17_GPIOB);

  for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
    bool value = (values >> digital_controls[control].pin) & 1;

    if (value != digital_controls[control].value) {
      digital_controls[control].value = value;
      ym2420_write_bit(digital_controls[control].ym2420_bit, value);

      if (print_to_lcd) {
        hd44780_print_digital_control(digital_controls[control]);
      }
    }
  }
}

void setup_controls() {
  mcp3008_setup(MCP3008_0_CS);
  mcp3008_setup(MCP3008_1_CS);

  apply_analog_controls(false);
  apply_digital_controls(false);
}

void apply_controls() {
  apply_analog_controls(true);
  apply_digital_controls(true);
}
