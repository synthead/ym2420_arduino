#include "tests.h"
#include "fm_synth.h"
#include "registers.h"

void print_binary(int value) {
  int bit_i;
  for (bit_i = 7; bit_i > -1; bit_i--) {
    Serial.print((value >> bit_i) % 2);
  }
  Serial.println();
}

void piano_note_test() {
  store_register_byte(0x10, 0b10101011);
  store_register_byte(0x30, 0b00110000);
  store_register_byte(0x20, 0b00011100);
  instrument(0, 3);
  write_registers();
  delay(1000);
  key_toggle(0);
  write_registers();
}

void all_88_keys() {
  instrument(0, 3);

  int key_i;
  for (key_i = 0; key_i < 88; key_i++) {
    key(0, key_i);
    key_toggle(0);
    rate_key_scale_toggle(0);
    write_registers();
    delay(100);

    key_toggle(0);
    write_registers();
  }
}

void all_f_numbers() {
  instrument(0, 4);
  octave(0, 5);
  key_toggle(0);

  int f_number_i;
  for (f_number_i = 0; f_number_i < 0b111111111; f_number_i++) {
    f_number(0, f_number_i);
    write_registers();
    delay(20);
  }
  key_toggle(0);
  write_registers();
}

void test_leds() {
  int bit_i;
  for (bit_i = 0; bit_i < 8; bit_i++) {
    apply_byte(1 << bit_i);
    delay(200);
  }
}
