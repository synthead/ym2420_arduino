#ifndef MCP23S17_H
#define MCP23S17_H

void mcp23s17_setup();
void mcp23s17_write(int, int, int);
int mcp23s17_read(int, int);

#endif
