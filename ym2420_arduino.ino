#include "ym2420.h"
#include "mcp23s17.h"
#include "hd44780.h"
#include "controls.h"
#include "keys.h"
#include "menu.h"
#include <SPI.h>

void setup() {
  SPI.begin();

  MCP23S17::setup();
  HD44780::setup();
  YM2420::setup();
  Controls::setup();

  Menu::startup_message();
}

void loop() {
  Menu::check_expired_message();
  Controls::apply();
  Keys::apply();
}
