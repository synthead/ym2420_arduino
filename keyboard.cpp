#include "keyboard.h"
#include "mcp23s17.h"
#include "ym2420.h"
#include <SPI.h>

#define MCP23S17_HW_ADDRESS 0b001

#define FIRST_KEY_NUMBER 46
#define OSCILLATORS 8

uint8_t active_oscillators[OSCILLATORS] = {0};
int active_keys[6][7] = {
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}};
uint8_t last_oscillator = 0;

void setup_keyboard(uint8_t instrument_number) {
  for (uint8_t oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
    ym2420_write_range(
        ym2420_oscillators[oscillator].instrument, instrument_number);
  }

  mcp23s17_write(MCP23S17_HW_ADDRESS, MCP23S17_IODIRB, 0x00);
}

void scan_keyboard() {
  for (uint8_t column = 0; column < 6; column++) {
    mcp23s17_write(MCP23S17_HW_ADDRESS, MCP23S17_GPIOB, 1 << column);
    uint8_t keys = mcp23s17_read(MCP23S17_HW_ADDRESS, MCP23S17_GPIOA);

    for (uint8_t row = 0; row < 7; row++) {
      bool key_on = (keys >> row) & 1;
      if (active_keys[column][row] == -1 && key_on) {
        for (uint8_t oscillator = 0; oscillator < OSCILLATORS; oscillator++) {
          uint8_t check_oscillator = (
              oscillator + last_oscillator + 1) % OSCILLATORS;

          if (active_oscillators[check_oscillator] == false) {
            ym2420_write_f_number_key(
                check_oscillator, row * 6 + column + FIRST_KEY_NUMBER);
            ym2420_write_bit(
                ym2420_oscillators[check_oscillator].key, true);

            last_oscillator = check_oscillator;
            active_oscillators[check_oscillator] = true;
            active_keys[column][row] = check_oscillator;

            break;
          }
        }
      } else if (active_keys[column][row] != -1 && ! key_on) {
        ym2420_write_bit(
            ym2420_oscillators[active_keys[column][row]].key, false);
        active_oscillators[active_keys[column][row]] = false;
        active_keys[column][row] = -1;
      }
    }
  }
}
