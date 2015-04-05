#ifndef ANALOG_H
#define ANALOG_H

#define MCP3008_RANGE 1023

#include <stdint.h>

namespace MCP3008 {
  void setup(uint8_t);
  uint8_t read_midi_value(uint8_t, uint8_t);
}

#endif
