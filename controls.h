#ifndef CONTROLS_H
#define CONTROLS_H

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
      const uint8_t midi_cc;
      const char* line1;
      const char* line2;
      uint8_t param_value;
      uint8_t midi_value;

      uint8_t get_midi_value();
      uint8_t get_param_value();
    public:
      AnalogControl(
          const uint8_t, const uint8_t, const uint8_t, YM2420::Range*,
          const char*, const char*);
      void set_current_value();
      void check_value();
  };

  class DigitalControl {
      const uint8_t pin;
      YM2420::Bit* ym2420_bit;
      const uint8_t midi_cc;
      const char* line1;
      const char* line2;
      bool value;

      bool read();
    public:
      DigitalControl(
          const uint8_t, const uint8_t, YM2420::Bit*, const char*, const char*);
      void set_current_value();
      void check_value();
  };
}

#endif
