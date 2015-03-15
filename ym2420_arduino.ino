#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include "controls.h"
#include "keyboard.h"
#include <SPI.h>

void setup() {
  SPI.begin();
  mcp23s17_setup();
  hd44780_setup();
  ym2420_setup();

  setup_controls();
  setup_keyboard(INSTRUMENT_ORIGINAL);
}

void loop() {
  apply_controls();
  scan_keyboard();
  ym2420_write_changes();
}
