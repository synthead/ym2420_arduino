#ifndef PATCH_STORAGE_H
#define PATCH_STORAGE_H

#include <stdbool.h>
#include <stdint.h>

#define PATCH_NAME_LENGTH 16

#define FIND_NEXT_ID -1

namespace PatchStorage {
  extern unsigned int current_id;
  extern char current_name[PATCH_NAME_LENGTH + 1];

  void setup();
  void new_patch();
  void print_patch();
  void write(uint32_t, char*);
  void read(uint32_t);
  void read_next();
  void read_previous();
}

#endif
