#ifndef PATCH_STORAGE_H
#define PATCH_STORAGE_H

#include <stdbool.h>
#include <stdint.h>

#define PATCH_NAME_LENGTH 16

namespace PatchStorage {
  void setup();
  char print_patch();
  void write(uint32_t, char*);
  void read(uint32_t);
  void read_next();
  void read_previous();
  uint32_t find_next_id();
}

#endif
