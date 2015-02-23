#ifndef REGISTERS_H
#define REGISTERS_H

#include <Arduino.h>

#define YM_D0 0
#define YM_D1 1
#define YM_D2 2
#define YM_D3 3
#define YM_D4 4
#define YM_D5 5
#define YM_D6 6
#define YM_D7 7

#define YM_A0 8
#define YM_CS 9
#define YM_IC 10

#define REGISTER_CHANGES_MAX 100
#define RESET_TIME 1000

extern int YM_D[8];
extern int registers[0x38];
extern int register_changes[REGISTER_CHANGES_MAX];
extern int register_changes_sum;

void write_registers();
void add_register_change(int);
void store_register_byte(int, int);
void toggle_register_bit(int, int);
void store_register_range(int, int, int, int);
void setup_pins();
void reset_registers();
void apply_byte(int);

#endif
