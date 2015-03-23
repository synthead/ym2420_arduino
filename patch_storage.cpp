#include "patch_storage.h"
#include "hd44780.h"
#include "ym2420.h"
#include "menu.h"
#include <Arduino.h>
#include <SD.h>

#define SDCARD_PRESENT 9
#define SDCARD_CS 8

#define PATCHES_DIRECTORY "YM2420"
#define PATCH_NAME_BYTES 17
#define PATCH_PATH_BYTES 22

namespace PatchStorage {
  struct current_patch_t {
    unsigned int id;
    char name[PATCH_NAME_BYTES];
    bool unsaved;
  };

  current_patch_t current_patch;

  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
    pinMode(SDCARD_CS, OUTPUT);
  }

  char print_patch() {
    char patch_number[17];
    sprintf(patch_number, "Patch %0d:", current_patch.id);
    HD44780::print_all(patch_number, (const char*)current_patch.name);
  }

  bool check_sdcard() {
    if (digitalRead(SDCARD_PRESENT)) {
      HD44780::print_all("No SD card", "inserted!");
      Menu::set_temporary_message();
      return false;
    }

    SD.begin(SDCARD_CS);
    return true;
  }

  bool check_patch_dir(bool create) {
    if (! SD.exists(PATCHES_DIRECTORY)) {
      if (create) {
        if (! SD.mkdir(PATCHES_DIRECTORY)) {
          char line2[17];
          sprintf(line2, "/%s dir!", PATCHES_DIRECTORY);
          HD44780::print_all("Problem creating", line2);
          Menu::set_temporary_message();
          return false;
        }
      } else {
        char line1[17];
        sprintf(line1, "/%s dir", PATCHES_DIRECTORY);
        HD44780::print_all(line1, "doesn't exist!");
        Menu::set_temporary_message();
        return false;
      }
    }

    File check_dir = SD.open(PATCHES_DIRECTORY);
    bool is_dir = check_dir.isDirectory();
    check_dir.close();

    return is_dir;
  }

  void write(uint32_t id, const char* name) {
    char path[PATCH_PATH_BYTES];
    sprintf(path, "%s/%08d.DAT", PATCHES_DIRECTORY, id);
    File patch = SD.open(path, O_WRITE | O_CREAT | O_TRUNC);

    char padded_name[PATCH_NAME_BYTES];
    sprintf(padded_name, "%-16s", name);
    patch.write(padded_name);

    for (uint8_t byte = 0; byte < YM2420_PATCH_RANGE; byte++) {
      patch.write(YM2420::register_contents[byte]);
    }

    patch.close();
    HD44780::print_all("Wrote patch", "to SD card.");
    Menu::set_temporary_message();
  }

  void read(uint32_t id) {
    char path[PATCH_PATH_BYTES];
    sprintf(path, "%s/%08d.DAT", PATCHES_DIRECTORY, id);
    File patch = SD.open(path);

    if (patch) {
      for (uint8_t byte = 0; byte < PATCH_NAME_BYTES - 1; byte++) {
        current_patch.name[byte] = patch.read();
      }

      for (uint8_t address = 0; address < YM2420_PATCH_RANGE; address++) {
        YM2420::register_contents[address] = patch.read();
        YM2420::write(address);
      }

      current_patch.id = id;
      print_patch();
    }

    patch.close();
  }

  uint32_t find_next_id() {
    uint32_t id = current_patch.id;

    while (++id) {
      char path[PATCH_PATH_BYTES];
      sprintf(path, "%s/%08d.DAT", PATCHES_DIRECTORY, id);
      if (! SD.exists(path)) {
        return id;
      }
    }
  }

  void read_next() {
    if (check_sdcard() && check_patch_dir(false)) {
      read(current_patch.id + 1);
    }
  }

  void read_previous() {
    if (current_patch.id != 0 && check_sdcard() && check_patch_dir(false)) {
      read(current_patch.id - 1);
    }
  }

  void write_next(const char* name) {
    if (check_sdcard() && check_patch_dir(true)) {
      uint32_t next_id = find_next_id();
      write(next_id, name);
    }
  }
}
