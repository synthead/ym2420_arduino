#include "registers.h"

int YM_D[8] = {YM_D0, YM_D1, YM_D2, YM_D3, YM_D4, YM_D5, YM_D6, YM_D7};

int registers[0x38] = {0};
int register_changes[REGISTER_CHANGES_MAX];
int register_changes_sum = 0;

void add_register_change(int address) {
  register_changes[register_changes_sum] = address;
  register_changes_sum++;
}

void store_register_byte(int address, int data) {
  registers[address] = data;
  add_register_change(address);
}

void toggle_register_bit(int address, int bit) {
  registers[address] ^= 1 << bit;
  add_register_change(address);
}

void store_register_range(int address, int data, int position, int length) {
  int mask = ((1 << length) - 1) << position;
  registers[address] = (registers[address] & ~mask) | (data << position & mask);
  add_register_change(address);
}

void setup_pins() {
  int pin_i;
  for (pin_i = 0; pin_i < 8; pin_i++) {
    pinMode(YM_D[pin_i], OUTPUT);
  }
  pinMode(YM_A0, OUTPUT);
  pinMode(YM_CS, OUTPUT);
  pinMode(YM_IC, OUTPUT);

  digitalWrite(YM_A0, 1);
  digitalWrite(YM_CS, 1);
  digitalWrite(YM_IC, 1);
}

void reset_registers() {
  digitalWrite(YM_IC, 0);
  delayMicroseconds(RESET_TIME);
  digitalWrite(YM_IC, 1);
}

void apply_byte(int data) {
  int pin_i;
  for (pin_i = 0; pin_i < 8; pin_i++) {
    digitalWrite(YM_D[pin_i], (data >> pin_i) % 2);
  }
}

void write_registers() {
  int change_i;
  for (change_i = 0; change_i < register_changes_sum; change_i++) {
    apply_byte(register_changes[change_i]);
    digitalWrite(YM_A0, 0);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);

    apply_byte(registers[register_changes[change_i]]);
    digitalWrite(YM_A0, 1);
    digitalWrite(YM_CS, 0);
    digitalWrite(YM_CS, 1);
  }

  register_changes_sum = 0;
}
