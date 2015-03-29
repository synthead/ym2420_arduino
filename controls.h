#ifndef CONTROLS_H
#define CONTROLS_H

#include "ym2420.h"

#include <stdint.h>
#include <stdbool.h>

namespace Controls {
  void setup();
  void update_all();

  class AnalogControl {
      const uint8_t chip_select;
      const uint8_t pin;
      YM2420::Range* ym2420_range;
      const char* line1;
      const char* line2;
      uint8_t value;

      void print();
    public:
      AnalogControl(
          const uint8_t, const uint8_t, YM2420::Range*, const char*,
          const char*);
      bool scan();
      void update();
  };

  class DigitalControl {
      const uint8_t pin;
      YM2420::Bit* ym2420_bit;
      const char* line1;
      const char* line2;
      bool value;

      void print();
    public:
      DigitalControl(const uint8_t, YM2420::Bit*, const char*, const char*);
      bool scan();
      void update();
  };
}

#endif
