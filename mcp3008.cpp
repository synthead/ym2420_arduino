#include "mcp3008.h"
#include <SPI.h>

void mcp3008_setup(int chip_select) {
  pinMode(chip_select, OUTPUT);
  digitalWrite(chip_select, HIGH);
};

int mcp3008_read(int chip_select, int channel, int range) {
  digitalWrite(chip_select, LOW);

  SPI.transfer((channel << 2) + 0b1100000);
  long value = SPI.transfer(0) << 2;
  value += (SPI.transfer(0) & 0b11000000) >> 6;

  digitalWrite(chip_select, HIGH);

  return value * range / 1023;
}
