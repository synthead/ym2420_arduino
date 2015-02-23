#include <SPI.h>
#include <MCP23S17.h>

#include "registers.h"
#include "fm_synth.h"
#include "analog_inputs.h"

void setup() {
  setup_pins();
  reset_registers();

  int oscillator_i;
  for (oscillator_i = 0; oscillator_i < 4; oscillator_i++) {
    octave(oscillator_i, 3);
    instrument(oscillator_i, 4);
    key_toggle(oscillator_i);
  }
}

void loop() {
  int oscillator_i;
  for (oscillator_i = 0; oscillator_i < 4; oscillator_i++) {
    analog_update_oscillator(
        oscillator_i, &f_number, oscillator_i, 0b111111111);
  }
  write_registers();
}
