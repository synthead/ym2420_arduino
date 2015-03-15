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
  const uint8_t range;
  void (*callback)(uint8_t);
  const char* line1;
  const char* line2;
  uint8_t value;
};

struct digital_control {
  const uint8_t pin;
  void (*callback)(bool);
  const char* line1;
  const char* line2;
  bool value;
};

struct analog_change last_analog_change = {
    .chip_select = -1,
    .pin = -1
};

uint8_t last_digital_change_pin = -1;

struct analog_control analog_controls[ANALOG_CONTROL_COUNT] = {
    {.chip_select = MCP3008_0_CS,
     .pin = 0,
     .range = RANGE_MULTI_SAMPLE_WAVE,
     .callback = multi_sample_wave_carrier,
     .line1 = "Multi-smple wave",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_0_CS,
     .pin = 1,
     .range = RANGE_MULTI_SAMPLE_WAVE,
     .callback = multi_sample_wave_modulation,
     .line1 = "Multi-smple wave",
     .line2 = "Modulation",
     .value = 0},

    {.chip_select = MCP3008_0_CS,
     .pin = 2,
     .range = RANGE_MODULATION_INDEX,
     .callback = modulation_index,
     .line1 = "Modulation index",
     .line2 = "",
     .value = 0},

    {.chip_select = MCP3008_0_CS,
     .pin = 3,
     .range = RANGE_FM_FEEDBACK_CONSTANT,
     .callback = fm_feedback_constant,
     .line1 = "FM feedback",
     .line2 = "",
     .value = 0},

    {.chip_select = MCP3008_0_CS,
     .pin = 4,
     .range = RANGE_LEVEL_KEY_SCALE,
     .callback = level_key_scale_carrier,
     .line1 = "LEVEL key scale",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_0_CS,
     .pin = 5,
     .range = RANGE_LEVEL_KEY_SCALE,
     .callback = level_key_scale_modulation,
     .line1 = "LEVEL key scale",
     .line2 = "Modulation",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 0,
     .range = RANGE_ADSR_RATE,
     .callback = attack_rate_carrier,
     .line1 = "Attack",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 1,
     .range = RANGE_ADSR_RATE,
     .callback = decay_rate_carrier,
     .line1 = "Decay",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 2,
     .range = RANGE_ADSR_RATE,
     .callback = sustain_rate_carrier,
     .line1 = "Sustain",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 3,
     .range = RANGE_ADSR_RATE,
     .callback = release_rate_carrier,
     .line1 = "Release",
     .line2 = "Carrier",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 4,
     .range = RANGE_ADSR_RATE,
     .callback = attack_rate_modulation,
     .line1 = "Attack",
     .line2 = "Modulation",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 5,
     .range = RANGE_ADSR_RATE,
     .callback = decay_rate_modulation,
     .line1 = "Decay",
     .line2 = "Modulation",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 6,
     .range = RANGE_ADSR_RATE,
     .callback = sustain_rate_modulation,
     .line1 = "Sustain",
     .line2 = "Modulation",
     .value = 0},

    {.chip_select = MCP3008_1_CS,
     .pin = 7,
     .range = RANGE_ADSR_RATE,
     .callback = release_rate_modulation,
     .line1 = "Release",
     .line2 = "Modulation",
     .value = 0}
};

struct digital_control digital_controls[DIGITAL_CONTROL_COUNT] = {
    {.pin = 0,
     .callback = amplitude_modulation_carrier,
     .line1 = "Amplitude Modltn",
     .line2 = "Carrier",
     .value = false},

    {.pin = 1,
     .callback = vibrato_carrier,
     .line1 = "Vibrato",
     .line2 = "Carrier",
     .value = false},

    {.pin = 2,
     .callback = wave_distortion_carrier,
     .line1 = "Wave distortion",
     .line2 = "Carrier",
     .value = false},

    {.pin = 3,
     .callback = rate_key_scale_carrier,
     .line1 = "RATE key scale",
     .line2 = "Carrier",
     .value = false},

    {.pin = 4,
     .callback = amplitude_modulation_modulation,
     .line1 = "Amplitude Modltn",
     .line2 = "Modulation",
     .value = false},

    {.pin = 5,
     .callback = vibrato_modulation,
     .line1 = "Vibrato",
     .line2 = "Modulation",
     .value = false},

    {.pin = 6,
     .callback = wave_distortion_modulation,
     .line1 = "Wave distortion",
     .line2 = "Modulation",
     .value = false},

    {.pin = 7,
     .callback = rate_key_scale_modulation,
     .line1 = "RATE key scale",
     .line2 = "Modulation",
     .value = false}
};

void lcd_print_analog_control(analog_control control) {
  char percent_text[5];
  sprintf(percent_text, "%3d%%", control.value * 100 / control.range);

  if (last_analog_change.chip_select != control.chip_select ||
      last_analog_change.pin != control.pin) {
    last_analog_change.chip_select = control.chip_select;
    last_analog_change.pin = control.pin;

    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    lcd_print_position(0, 0, line1_padded);

    char line2_padded[13];
    sprintf(line2_padded, "%-12s", control.line2);
    lcd_print_position(0, 1, line2_padded);

    lcd_print(percent_text);
  } else {
    lcd_print_position(12, 1, percent_text);
  }
}

void lcd_print_digital_control(digital_control control) {
  const char* state = control.value ? "On " : "Off";

  if (last_digital_change_pin != control.pin) {
    last_digital_change_pin = control.pin;

    char line1_padded[17];
    sprintf(line1_padded, "%-16s", control.line1);
    lcd_print_position(0, 0, line1_padded);

    char line2_padded[14];
    sprintf(line2_padded, "%-13s", control.line2);
    lcd_print_position(0, 1, line2_padded);

    lcd_print(state);
  } else {
    lcd_print_position(13, 1, state);
  }
}

void apply_analog_controls(bool print_to_lcd) {
  for (uint8_t control = 0; control < ANALOG_CONTROL_COUNT; control++) {
    unsigned int value = mcp3008_read(
        analog_controls[control].chip_select,
        analog_controls[control].pin,
        analog_controls[control].range);

    if (value != analog_controls[control].value) {
      analog_controls[control].value = value;
      analog_controls[control].callback(value);

      if (print_to_lcd) {
        lcd_print_analog_control(analog_controls[control]);
      }
    }
  }
}

void apply_digital_controls(bool print_to_lcd) {
  uint8_t values = mcp23s17_read(MCP23S17_HW_ADDRESS, 0x13);

  for (uint8_t control = 0; control < DIGITAL_CONTROL_COUNT; control++) {
    bool value = (values >> digital_controls[control].pin) & 1;

    if (value != digital_controls[control].value) {
      digital_controls[control].value = value;
      digital_controls[control].callback(value);

      if (print_to_lcd) {
        lcd_print_digital_control(digital_controls[control]);
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
