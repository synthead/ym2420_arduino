#ifndef PATCH_STORAGE_H
#define PATCH_STORAGE_H

#include <stdbool.h>  // For bool.

namespace PatchStorage {
  void setup();
  char print_patch();
  void write(unsigned int, const char*);
  void read(unsigned int);
}

#endif
