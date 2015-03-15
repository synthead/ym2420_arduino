#include "mcp23s17.h"
#include <Arduino.h>
#include <SPI.h>

#define MCP23S17_CS 5

void mcp23s17_setup() {
  pinMode(MCP23S17_CS, OUTPUT);
  digitalWrite(MCP23S17_CS, HIGH);
  mcp23s17_write(0b000, 0x0a, 0b00001000);  // Enable IOCON.HAEN.
}

void mcp23s17_write(uint8_t hw_address, uint8_t address, uint8_t data) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000000 | hw_address << 1);
  SPI.transfer(address);
  SPI.transfer(data);

  digitalWrite(MCP23S17_CS, HIGH);
}

uint8_t mcp23s17_read(uint8_t hw_address, uint8_t address) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000001 | hw_address << 1);
  SPI.transfer(address);
  uint8_t value = SPI.transfer(0);

  digitalWrite(MCP23S17_CS, HIGH);

  return value;
}

