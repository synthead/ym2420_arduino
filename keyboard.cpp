#include "keyboard.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include <SPI.h>

#define MCP23S17_HW_ADDRESS 0b001
#define FIRST_KEY_NUMBER 40
#define OSCILLATORS 8

int active_oscillators[OSCILLATORS] = {0};
int active_keys[6][7] = {
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}};
int last_oscillator = 0;

void setup_keyboard(int instrument_number) {
  for (int oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
    instrument(oscillator, instrument_number);
  }

  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x01, 0x00);
}

void scan_keyboard() {
  for (int column = 0; column < 6; column++) {
    mcp23s17_write(MCP23S17_HW_ADDRESS, 0x13, 1 << column);
    int keys = mcp23s17_read(MCP23S17_HW_ADDRESS, 0x12);

    for (int row = 0; row < 7; row++) {
      int key_on = (keys >> row) % 2;
      if (active_keys[column][row] == -1 && key_on) {
        for (int oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
          int check_oscillator = (oscillator + last_oscillator + 1) % OSCILLATORS;

          if (active_oscillators[check_oscillator] == 0) {
            key(check_oscillator, row * 6 + column + FIRST_KEY_NUMBER);
            toggle_oscillator(check_oscillator);

            last_oscillator = check_oscillator;
            active_oscillators[check_oscillator] = 1;
            active_keys[column][row] = check_oscillator;

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
