#include "mcp23s17.h"
#include <Arduino.h>
#include <SPI.h>

#define MCP23S17_CS 5

void mcp23s17_setup() {
  pinMode(MCP23S17_CS, OUTPUT);
  digitalWrite(MCP23S17_CS, HIGH);
  mcp23s17_write(0b000, 0x0a, 0b00001000);  // Enable IOCON.HAEN.
}

void mcp23s17_write(int hw_address, int address, int data) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000000 | hw_address << 1);
  SPI.transfer(address);
  SPI.transfer(data);

  digitalWrite(MCP23S17_CS, HIGH);
}

int mcp23s17_read(int hw_address, int address) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000001 | hw_address << 1);
  SPI.transfer(address);
  int slave = SPI.transfer(0);

  digitalWrite(MCP23S17_CS, HIGH);
  return slave;
}

