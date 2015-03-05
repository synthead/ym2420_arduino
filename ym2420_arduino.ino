#include "ym2420.h"
#include "keyboard.h"
#include "mcp23s17.h"
// #include "analog.h"
#include <SPI.h>

void setup() {
  SPI.begin();
  mcp23s17_setup();
  ym2420_setup();
  ym2420_reset();
  setup_keyboard(INSTRUMENT_PIANO);
}

void loop() {
  scan_keyboard();
  ym2420_write_changes();
}
