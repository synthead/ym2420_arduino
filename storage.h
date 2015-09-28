#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdint.h>

#define PATCH_NAME_LENGTH 16
#define FIND_NEXT_ID -1

#define MIDI_SETTINGS_FILE_BYTES 23

namespace Storage {
  void setup();
  bool check_sdcard(bool);

  namespace MIDISettings {
    void setup();
    bool write();
  }

  namespace Patches {
    extern unsigned int current_id;
    extern char current_name[PATCH_NAME_LENGTH + 1];

    void new_patch();
    void print_patch();
    bool write(uint32_t, char*);
    void read(uint32_t);
    void read_next();
    void read_previous();
  }
}

#endif
