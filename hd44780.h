#ifndef LCD_H
#define LCD_H

void lcd_print(const char*);
void lcd_print_position(int, int, const char*);
void hd44780_setup();

#endif
