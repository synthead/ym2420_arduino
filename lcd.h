#ifndef LCD_H
#define LCD_H

void lcd_print(const char*);
void lcd_print_position(int, int, const char*);
void lcd_print_analog_control(const char*, const char*, int, int);
void lcd_print_digital_control(const char*, const char*, int);
void lcd_setup();

#endif
