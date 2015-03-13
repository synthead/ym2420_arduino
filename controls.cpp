#include "mcp3008.h"
#include "mcp23s17.h"
#include "ym2420.h"

#define MCP3008_0_CS 6
#define MCP3008_1_CS 7
#define MCP23S17_HW_ADDRESS 0b000

void setup_analog_controls() {
  mcp3008_setup(MCP3008_0_CS);
  mcp3008_setup(MCP3008_1_CS);
}

void scan_analog_controls() {
  multi_sample_wave(
      CARRIER, mcp3008_read(MCP3008_0_CS, 0, RANGE_MULTI_SAMPLE_WAVE));
  multi_sample_wave(
      MODULATION, mcp3008_read(MCP3008_0_CS, 1, RANGE_MULTI_SAMPLE_WAVE));
  modulation_index(mcp3008_read(MCP3008_0_CS, 2, RANGE_MODULATION_INDEX));
  fm_feedback_constant(
      mcp3008_read(MCP3008_0_CS, 3, RANGE_FM_FEEDBACK_CONSTANT));
  level_key_scale(
      CARRIER, mcp3008_read(MCP3008_0_CS, 4, RANGE_LEVEL_KEY_SCALE));
  level_key_scale(
      MODULATION, mcp3008_read(MCP3008_0_CS, 5, RANGE_LEVEL_KEY_SCALE));

  attack_rate(CARRIER, mcp3008_read(MCP3008_1_CS, 0, RANGE_ATTACK_RATE));
  decay_rate(CARRIER, mcp3008_read(MCP3008_1_CS, 1, RANGE_DECAY_RATE));
  sustain_rate(CARRIER, mcp3008_read(MCP3008_1_CS, 2, RANGE_SUSTAIN_RATE));
  release_rate(CARRIER, mcp3008_read(MCP3008_1_CS, 3, RANGE_RELEASE_RATE));
  attack_rate(MODULATION, mcp3008_read(MCP3008_1_CS, 4, RANGE_ATTACK_RATE));
  decay_rate(MODULATION, mcp3008_read(MCP3008_1_CS, 5, RANGE_DECAY_RATE));
  sustain_rate(MODULATION, mcp3008_read(MCP3008_1_CS, 6, RANGE_SUSTAIN_RATE));
  release_rate(MODULATION, mcp3008_read(MCP3008_1_CS, 7, RANGE_RELEASE_RATE));
}

void scan_digital_controls() {
  int switches = mcp23s17_read(MCP23S17_HW_ADDRESS, 0x13);

  amplitude_modulation(CARRIER, switches % 2);
  amplitude_modulation(MODULATION, (switches >> 1) % 2);
  vibrato(CARRIER, (switches >> 2) % 2);
  vibrato(MODULATION, (switches >> 3) % 2);
  rate_key_scale(CARRIER, (switches >> 4) % 2);
  rate_key_scale(MODULATION, (switches >> 5) % 2);
  wave_distortion(CARRIER, (switches >> 6) % 2);
  wave_distortion(MODULATION, (switches >> 7) % 2);
};
