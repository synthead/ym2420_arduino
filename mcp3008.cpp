#include "mcp3008.h"
#include <SPI.h>

void mcp3008_setup(uint8_t chip_select) {
  pinMode(chip_select, OUTPUT);
  digitalWrite(chip_select, HIGH);
};

unsigned int mcp3008_read(uint8_t chip_select, uint8_t channel, uint8_t range) {
  digitalWrite(chip_select, LOW);

  SPI.transfer((channel << 2) + 0b01100000);
  unsigned int value = SPI.transfer(0) << 2;
  value += (SPI.transfer(0) & 0b11000000) >> 6;

  digitalWrite(chip_select, HIGH);

  return value * range / 1023;
}
