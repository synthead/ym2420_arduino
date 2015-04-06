#ifndef CONTROLS_H
#define CONTROLS_H

#define ANALOG_CONTROL_COUNT 14
#define DIGITAL_CONTROL_COUNT 8

#include "ym2420.h"

#include <stdint.h>
#include <stdbool.h>

namespace Controls {
  void setup();
  void set_current_values();
  void check_values();

  class AnalogControl {
      const uint8_t chip_select;
      const uint8_t pin;
      YM2420::Range* ym2420_range;
      uint8_t* cc_number;
      const char* line_1;
      const char* line_2;
      uint8_t param_value;
      uint8_t midi_value;

      uint8_t get_midi_value();
      uint8_t get_param_value();
    public:
      AnalogControl(
          const uint8_t, const uint8_t, uint8_t*, YM2420::Range*, const char*,
          const char*);
      void set_current_value();
      void check_value();
      bool set_via_cc_number(uint8_t, uint8_t);
  };

  class DigitalControl {
      const uint8_t pin;
      YM2420::Bit* ym2420_bit;
      uint8_t* cc_number;
      const char* line_1;
      const char* line_2;
      bool value;

      bool read();
    public:
      DigitalControl(
          const uint8_t, uint8_t*, YM2420::Bit*, const char*, const char*);
      void set_current_value();
      void check_value();
      bool set_via_cc_number(uint8_t, uint8_t);
  };

  extern AnalogControl analog_controls[ANALOG_CONTROL_COUNT];
  extern DigitalControl digital_controls[DIGITAL_CONTROL_COUNT];
}

#endif
