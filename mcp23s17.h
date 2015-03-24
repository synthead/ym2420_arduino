#ifndef MCP23S17_H
#define MCP23S17_H

#include <stdint.h>

#define MCP23S17_GPIOA 0x12
#define MCP23S17_GPIOB 0x13

namespace MCP23S17 {
  void setup();
  void write(uint8_t, uint8_t);
  uint8_t read(uint8_t);
  void write_parallel(uint8_t, bool, uint8_t);
  uint8_t scan_matrix(uint8_t);
}

#endif
