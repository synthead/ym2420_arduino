#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

namespace MCP3008 {
  void setup(uint8_t);
  unsigned int read(uint8_t, uint8_t, uint8_t);
}

#endif
