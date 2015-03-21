#include "hd44780.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define LCD_E 4

#define CHARACTER_MODE true
#define COMMAND_MODE false

#define HD44780_LINE2 0x40
#define HD44780_SET_POSITON 0x80

void hd44780_print(const char* text) {
  uint8_t text_length = strlen(text);
  for (uint8_t cell = 0; cell < text_length; cell++) {
    mcp23s17_write_parallel(LCD_E, CHARACTER_MODE, text[cell]);
  }
}

void hd44780_print_position(uint8_t column, uint8_t row, const char* text) {
  mcp23s17_write_parallel(
      LCD_E, COMMAND_MODE, HD44780_SET_POSITON | HD44780_LINE2 * row + column);
  hd44780_print(text);
}

void hd44780_setup() {
  pinMode(LCD_E, OUTPUT);
  digitalWrite(LCD_E, HIGH);

  mcp23s17_write_parallel(LCD_E, COMMAND_MODE, 0x38);
  mcp23s17_write_parallel(LCD_E, COMMAND_MODE, 0x0C);
}
