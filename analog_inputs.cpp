#include "analog_inputs.h"

int analog_average(int analog_pin, int range) {
  int sample_i;
  long sample_sum = 0;
  for (sample_i = 0; sample_i < ANALOG_SAMPLES; sample_i++) {
    sample_sum += analogRead(analog_pin);
  }
  return (sample_sum / ANALOG_SAMPLES) * range / 1023;
}

void analog_update(int analog_pin, void (*callback)(int), int range) {
  callback(analog_average(analog_pin, range));
}

void analog_update_oscillator(
    int analog_pin, void (*callback)(int, int), int oscillator, int range) {
  callback(oscillator, analog_average(analog_pin, range));
}
