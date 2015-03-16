#include "mcp23s17.h"
#include <Arduino.h>
#include <SPI.h>

#define MCP23S17_CS 5

#define MCP23S17_GPIOA 0x12
#define MCP23S17_GPIOB 0x13

#define MCP23S17_IOCON 0x0a
#define MCP23S17_IOCON_HAEN 0b00001000

#define PARALLEL_BUS_HW_ADDRESS 0b000
#define PARALLEL_BUS_GPIO MCP23S17_GPIOA
#define PARALLEL_BUS_ADDRESS_MODE 2

void mcp23s17_setup() {
  pinMode(MCP23S17_CS, OUTPUT);
  pinMode(PARALLEL_BUS_ADDRESS_MODE, OUTPUT);

  digitalWrite(MCP23S17_CS, HIGH);

  mcp23s17_write(0b000, MCP23S17_IOCON, MCP23S17_IOCON_HAEN);
  mcp23s17_write(PARALLEL_BUS_HW_ADDRESS, MCP23S17_IODIRA, 0x00);
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

void mcp23s17_write_parallel(uint8_t chip_select, bool mode, uint8_t data) {
  digitalWrite(PARALLEL_BUS_ADDRESS_MODE, mode);

  mcp23s17_write(PARALLEL_BUS_HW_ADDRESS, PARALLEL_BUS_GPIO, data);

  digitalWrite(chip_select, LOW);
  digitalWrite(chip_select, HIGH);
}
