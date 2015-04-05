#include "mcp3008.h"
#include "midi.h"
#include <SPI.h>

namespace MCP3008 {
  void setup(uint8_t chip_select) {
    pinMode(chip_select, OUTPUT);
    digitalWrite(chip_select, HIGH);
  };

  uint8_t read_midi_value(uint8_t chip_select, uint8_t channel) {
    digitalWrite(chip_select, LOW);

    SPI.transfer((channel << 2) + 0b01100000);
    uint8_t value = (SPI.transfer(0) << 2) / 8;
    value += (((SPI.transfer(0) & 0b11000000) >> 6) + 4) / 8;

    digitalWrite(chip_select, HIGH);

    return value;
  }
}
