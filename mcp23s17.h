#ifndef MCP23S17_H
#define MCP23S17_H

#include <stdint.h>  // For uint8_t.

#define MCP23S17_IODIRA 0x00
#define MCP23S17_IODIRB 0x01

#define MCP23S17_GPIOA 0x12
#define MCP23S17_GPIOB 0x13

void mcp23s17_setup();
void mcp23s17_write(uint8_t, uint8_t, uint8_t);
uint8_t mcp23s17_read(uint8_t, uint8_t);
void mcp23s17_write_parallel(uint8_t, bool, uint8_t);

#endif
