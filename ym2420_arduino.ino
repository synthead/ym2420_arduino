#include "ym2420.h"
#include "keyboard.h"
#include "mcp23s17.h"
#include "controls.h"
#include "lcd.h"
#include <SPI.h>

void setup() {
  SPI.begin();
  mcp23s17_setup();
  lcd_setup();
  setup_analog_controls();
  ym2420_setup();
  ym2420_reset();
  setup_keyboard(INSTRUMENT_ORIGINAL);

  lcd_print(0, 0, "Synthead YM2420");
  lcd_print(0, 1, "SW version 0.1");
}

void loop() {
  scan_analog_controls();
  scan_digital_controls();
  scan_keyboard();
  ym2420_write_changes();
}
