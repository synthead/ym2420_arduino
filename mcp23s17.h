#ifndef MCP23S17_H
#define MCP23S17_H

#include <stdint.h>  // For uint8_t.

void mcp23s17_setup();
void mcp23s17_write(uint8_t, uint8_t, uint8_t);
uint8_t mcp23s17_read(uint8_t, uint8_t);

#endif
