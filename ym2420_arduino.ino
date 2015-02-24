#include <SPI.h>
#include <MCP23S17.h>

#include "registers.h"
#include "fm_synth.h"
#include "analog_inputs.h"

void setup() {
  setup_pins();
  reset_registers();

  octave(0, 3);
  f_number(0, 288);
  instrument(0, 4);
  key_toggle(0);
}

void loop() {
  f_number(0, mcp3008_average(MCP3008_0_CS, 0, F_NUMBER_RANGE));
  write_registers();
}
