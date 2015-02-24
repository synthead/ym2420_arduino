#ifndef ANALOG_INPUTS_H
#define ANALOG_INPUTS_H

#include "fm_synth.h"
#include "registers.h"

#define ANALOG_SAMPLES 5
#define MCP3008_CS_WAIT 4
#define MCP3008_0_CS A0

void mcp3008_setup();
int mcp3008_read(int, int);
int mcp3008_average(int, int, int);

#endif
