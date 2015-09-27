#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include "controls.h"
#include "keys.h"
#include "menu.h"
#include "storage.h"
#include "midi.h"
#include <SPI.h>
#include <SD.h>

void setup() {
  SPI.begin();
  Serial1.begin(31250);

  MCP23S17::setup();
  HD44780::setup();
  YM2420::setup();
  Controls::setup();
  Storage::setup();
  MIDI::setup();

  Menu::startup_message();
}

void loop() {
  Menu::check_expired_message();
  Menu::scan_inputs();
  Controls::check_values();
  Keys::apply();
  MIDI::process_messages();
}
