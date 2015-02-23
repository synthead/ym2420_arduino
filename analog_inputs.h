#ifndef ANALOG_INPUTS_H
#define ANALOG_INPUTS_H

#include "fm_synth.h"
#include "registers.h"

#define ANALOG_SAMPLES 10

int analog_average(int, int);
void analog_update(int, void (*)(int), int);
void analog_update_oscillator(int, void (*)(int, int), int, int);

#endif
