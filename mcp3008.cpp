#include "mcp3008.h"
#include <SPI.h>

#define MCP3008_CS_WAIT 4
#define MCP3008_SAMPLES 10

void mcp3008_setup(int chip_select) {
  pinMode(chip_select, OUTPUT);
  digitalWrite(chip_select, HIGH);
};

int mcp3008_read(int chip_select, int channel, int range) {
  long sample_sum = 0;

  for (int sample = 0; sample < MCP3008_SAMPLES; sample++) {
    digitalWrite(chip_select, LOW);
    delayMicroseconds(MCP3008_CS_WAIT);

    SPI.transfer((channel << 2) + 0b1100000);
    sample_sum += SPI.transfer(0) << 2;
    sample_sum += (SPI.transfer(0) & 0b11000000) >> 6;

    digitalWrite(chip_select, HIGH);
  }

  return (sample_sum / MCP3008_SAMPLES) * range / 1023;
}
