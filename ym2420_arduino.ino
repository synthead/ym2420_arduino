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
}

void loop() {
  check_expired_message();
  apply_controls();
  scan_keyboard();
}
