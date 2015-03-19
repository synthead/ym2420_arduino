#include "hd44780.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define LCD_E 4

#define CHARACTER_MODE true
#define COMMAND_MODE false

#define HD44780_LINE2 0x40
#define HD44780_SET_POSITON 0x80

#define TEMP_MESSAGE_EXPIRATION 3000

temporary_message_t temporary_message;

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

void set_temporary_message() {
  temporary_message.displayed_at = millis();
  temporary_message.expired = false;
}

void check_expired_message() {
  if (! temporary_message.expired &&
      temporary_message.displayed_at + TEMP_MESSAGE_EXPIRATION < millis()) {
    temporary_message.expired = true;

    hd44780_print_position(0, 0, "000: I made it! ");
    hd44780_print_position(0, 1, "                ");
  }
}

void hd44780_setup() {
  pinMode(LCD_E, OUTPUT);
  digitalWrite(LCD_E, HIGH);

  mcp23s17_write_parallel(LCD_E, COMMAND_MODE, 0x38);
  mcp23s17_write_parallel(LCD_E, COMMAND_MODE, 0x0C);

  hd44780_print_position(0, 0, "Synthead YM2420 ");
  hd44780_print_position(0, 1, "SW version 0.1  ");

  set_temporary_message();
}
