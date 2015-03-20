#include "mcp23s17.h"
#include <Arduino.h>
#include <SPI.h>

#define MCP23S17_CS 5

#define MCP23S17_IODIRA 0x00
#define MCP23S17_ALL_WRITE 0x00

#define MCP23S17_IOCON 0x0a
#define MCP23S17_IOCON_HAEN 0b00001000

#define PARALLEL_BUS_GPIO MCP23S17_GPIOA
#define PARALLEL_BUS_ADDRESS_MODE 2

void mcp23s17_setup() {
  pinMode(MCP23S17_CS, OUTPUT);
  pinMode(PARALLEL_BUS_ADDRESS_MODE, OUTPUT);

  digitalWrite(MCP23S17_CS, HIGH);

  mcp23s17_write(MCP23S17_IOCON, MCP23S17_IOCON_HAEN);
  mcp23s17_write(MCP23S17_IODIRA, MCP23S17_ALL_WRITE);
}

void mcp23s17_write(uint8_t address, uint8_t data) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000000);
  SPI.transfer(address);
  SPI.transfer(data);

  digitalWrite(MCP23S17_CS, HIGH);
}

uint8_t mcp23s17_read(uint8_t address) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000001);
  SPI.transfer(address);
  uint8_t value = SPI.transfer(0);

  digitalWrite(MCP23S17_CS, HIGH);

  return value;
}

void mcp23s17_write_parallel(uint8_t chip_select, bool mode, uint8_t data) {
  digitalWrite(PARALLEL_BUS_ADDRESS_MODE, mode);

  mcp23s17_write(PARALLEL_BUS_GPIO, data);

  digitalWrite(chip_select, LOW);
  digitalWrite(chip_select, HIGH);
}

uint8_t mcp23s17_scan_matrix(uint8_t high_bit) {
  mcp23s17_write(MCP23S17_GPIOA, 1 << high_bit);
  return mcp23s17_read(MCP23S17_GPIOB);
}
