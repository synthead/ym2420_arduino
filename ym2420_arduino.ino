#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include "controls.h"
#include "keys.h"
#include "menu.h"
#include "patch_storage.h"
#include <SPI.h>
#include <SD.h>

void setup() {
  SPI.begin();

  MCP23S17::setup();
  HD44780::setup();
  YM2420::setup();
  Controls::setup();
  PatchStorage::setup();

  Menu::startup_message();
  PatchStorage::read(0);
}

void loop() {
  Menu::check_expired_message();
  Menu::scan_inputs();
  Controls::apply();
  Keys::apply();
}
