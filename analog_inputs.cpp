#include "analog_inputs.h"
#include <SPI.h>

void mcp3008_setup() {
  pinMode(MCP3008_0_CS, OUTPUT);
  digitalWrite(MCP3008_0_CS, HIGH);
};

int mcp3008_read(int chip_select, int channel) {
  digitalWrite(chip_select, LOW);
  delayMicroseconds(MCP3008_CS_WAIT);

  SPI.transfer((channel << 2) + 0b1100000);
  int value = SPI.transfer(0) << 2;
  value += (SPI.transfer(0) & 0b11000000) >> 6;

  digitalWrite(chip_select, HIGH);
  return value;
}

int mcp3008_average(int chip_select, int channel, int range) {
  int sample_i;
  long sample_sum = 0;
  for (sample_i = 0; sample_i < ANALOG_SAMPLES; sample_i++) {
    sample_sum += mcp3008_read(chip_select, channel);
  }
  return (sample_sum / ANALOG_SAMPLES) * range / 1023;
}

// int native_read(int analog_pin, int range) {
//   long sample_sum = 0;
//   int sample_i;
//   for (sample_i = 0; sample_i < ANALOG_SAMPLES; sample_i++) {
//     sample_sum += analogRead(analog_pin);
//   }
//   return (sample_sum / ANALOG_SAMPLES) * range / 1023;
// }
