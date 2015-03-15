#include "hd44780.h"
#include "mcp23s17.h"
#include <Arduino.h>

#define MCP23S17_HW_ADDRESS 0b010

#define LCD_RS 0b00000001
#define LCD_E 0b00000010

void write_command(int command) {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, LCD_E);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x13, command);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, 0);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, LCD_E);
}

void write_character(char character) {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, LCD_E | LCD_RS);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x13, character);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, LCD_RS);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x12, LCD_E | LCD_RS);
}

void lcd_print(const char* text) {
  int text_length = strlen(text);
  for (int cell = 0; cell < text_length; cell++) {
    write_character(text[cell]);
  }
}

void lcd_print_position(int column, int row, const char* text) {
  write_command(0x80 + 0x40 * row + column);
  lcd_print(text);
}

void hd44780_setup() {
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x00, 0x00);
  mcp23s17_write(MCP23S17_HW_ADDRESS, 0x01, 0x00);

  write_command(0x38);
  write_command(0x0C);

  lcd_print_position(0, 0, "Synthead YM2420");
  lcd_print_position(0, 1, "SW version 0.1");
}
