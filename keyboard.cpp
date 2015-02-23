#include "keyboard.h"
#include "fm_synth.h"

MCP keyboard(1);

int active_keys[7][6] = {-1};
int active_oscillators[OSCILLATOR_COUNT] = {0};

void scan_keyboard() {
  int column;
  for (column = 0; column < 6; column++) {
    keyboard.byteWrite(0x12, 1 << column);
    int keys = keyboard.byteRead(0x13);

    int row;
    for (row = 0; row < 7; row++) {
      int key_on = (keys >> row) % 2;

      if (active_keys[column][row] == -1 && key_on) {
        for (int oscillator = 0; oscillator < OSCILLATOR_COUNT; oscillator++) {
          if (active_oscillators[oscillator] == false) {
            key(oscillator, row + column * 7 - 6);
            toggle_oscillator(oscillator);

            active_oscillators[oscillator] = true;
            active_keys[column][row] = oscillator;
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

