#include "keyboard.h"
#include "ym2420.h"
#include <SPI.h>

#define MCP23S17_CS A1
#define MCP23S17_CS_WAIT 20

#define FIRST_KEY_NUMBER 40
#define OSCILLATORS 8

int active_oscillators[OSCILLATORS] = {0};
int active_keys[6][7] = {
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}};

void mcp23s17_write(int address, int data) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000000);
  SPI.transfer(address);
  SPI.transfer(data);

  digitalWrite(MCP23S17_CS, HIGH);
}

int mcp23s17_read(int address) {
  digitalWrite(MCP23S17_CS, LOW);

  SPI.transfer(0b01000001);
  SPI.transfer(address);
  int slave = SPI.transfer(0);

  digitalWrite(MCP23S17_CS, HIGH);
  return slave;
}

void setup_keyboard(int instrument_number) {
  for (int oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
    instrument(oscillator, instrument_number);
  }

  pinMode(MCP23S17_CS, OUTPUT);
  digitalWrite(MCP23S17_CS, HIGH);
  mcp23s17_write(0x01, 0x00);
}

void scan_keyboard() {
  for (int column = 0; column < 6; column++) {
    mcp23s17_write(0x13, 1 << column);
    int keys = mcp23s17_read(0x12);

    for (int row = 0; row < 7; row++) {
      int key_on = (keys >> row) % 2;
      if (active_keys[column][row] == -1 && key_on) {
        for (int oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
          if (active_oscillators[oscillator] == 0) {
            key(oscillator, row * 6 + column + FIRST_KEY_NUMBER);
            toggle_oscillator(oscillator);

            active_oscillators[oscillator] = 1;
            active_keys[column][row] = oscillator;

            break;
          }
        }
      } else if (active_keys[column][row] != -1 && ! key_on) {
        toggle_oscillator(active_keys[column][row]);

        active_oscillators[active_keys[column][row]] = 0;
        active_keys[column][row] = -1;
      }
    }
  }
}
