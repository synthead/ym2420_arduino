#ifndef PATCH_STORAGE_H
#define PATCH_STORAGE_H

#include <stdbool.h>  // For bool.
#include <stdint.h>  // For uint32_t.

namespace PatchStorage {
  void setup();
  char print_patch();
  void write(uint32_t, const char*);
  void read(uint32_t);
  void read_next();
  void read_previous();
  void write_next(const char*);
}

#endif
