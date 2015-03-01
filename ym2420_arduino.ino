#include "ym2420.h"
#include "keyboard.h"
#include "analog.h"
#include <SPI.h>

void setup() {
  SPI.begin();
  setup_ym2420();
  reset_ym2420();
  setup_keyboard(INSTRUMENT_PIANO);
}

void loop() {
  scan_keyboard();
  write_ym2420_changes();
}
