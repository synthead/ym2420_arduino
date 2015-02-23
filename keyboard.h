#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SPI.h>
#include <MCP23S17.h>

#define OSCILLATOR_COUNT 9
#define FIRST_KEY_NUMBER 36

extern MCP keyboard;

extern int active_keys[7][6];
extern int active_oscillators[OSCILLATOR_COUNT];

void scan_keyboard();

#endif
