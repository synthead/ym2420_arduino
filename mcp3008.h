#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>  // For uint8_t.

void mcp3008_setup(uint8_t);
unsigned int mcp3008_read(uint8_t, uint8_t, uint8_t);

#endif
