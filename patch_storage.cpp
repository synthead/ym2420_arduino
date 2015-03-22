#include "patch_storage.h"
#include "hd44780.h"
#include "ym2420.h"
#include "menu.h"
#include <Arduino.h>
#include <SD.h>

#define SDCARD_PRESENT 9
#define SDCARD_CS 8

#define PATCHES_DIRECTORY "YM2420"
#define PATCH_NAMES_LENGTH 16

namespace PatchStorage {
  struct current_patch_t {
    unsigned int id;
    char name[PATCH_NAMES_LENGTH + 1];
    bool unsaved;
    uint8_t ym2420_patch[YM2420_PATCH_RANGE];
  };

  current_patch_t current_patch;

  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
    pinMode(SDCARD_CS, OUTPUT);

    SD.begin(SDCARD_CS);
  }

  char print_patch() {
    char patch_number[17];
    sprintf(patch_number, "Patch %0d:", current_patch.id);
    HD44780::print_all(patch_number, (const char*)current_patch.name);
  }

  void write(unsigned int id, const char* name) {
    if (digitalRead(SDCARD_PRESENT)) {
      HD44780::print_all("No SD card", "inserted!");
      Menu::set_temporary_message();
      return;
    }

    SD.begin(SDCARD_CS);

    if (! SD.exists(PATCHES_DIRECTORY)) {
      if (! SD.mkdir(PATCHES_DIRECTORY)) {
        char directory[17];
        sprintf(directory, "/%s dir!", PATCHES_DIRECTORY);
        HD44780::print_all("Problem creating", directory);
        Menu::set_temporary_message();
        return;
      }
    }

    char path[22];
    sprintf(path, "%s/%08d.DAT", PATCHES_DIRECTORY, id);
    File patch = SD.open(path, O_WRITE | O_CREAT | O_TRUNC);

    patch.write(name);

    for (uint8_t byte = 0; byte < YM2420_PATCH_RANGE; byte++) {
      patch.write(YM2420::register_contents[byte]);
    }

    patch.close();

    HD44780::print_all("Wrote patch", "to SD card.");
    Menu::set_temporary_message();
  }

  void read(unsigned int id) {
    if (digitalRead(SDCARD_PRESENT)) {
      HD44780::print_all("No SD card", "inserted!");
      Menu::set_temporary_message();
      return;
    }

    SD.begin(SDCARD_CS);

    if (! SD.exists(PATCHES_DIRECTORY)) {
      if (! SD.mkdir(PATCHES_DIRECTORY)) {
        char directory[17];
        sprintf(directory, "/%s dir", PATCHES_DIRECTORY);
        HD44780::print_all(directory, "doesn't exist!");
        Menu::set_temporary_message();
        return;
      }
    }

    char path[22];
    sprintf(path, "%s/%08d.DAT", PATCHES_DIRECTORY, id);
    File patch = SD.open(path);

    for (uint8_t byte = 0; byte < PATCH_NAMES_LENGTH; byte++) {
      current_patch.name[byte] = patch.read();
    }

    for (uint8_t byte = 0; byte < YM2420_PATCH_RANGE; byte++) {
      current_patch.ym2420_patch[byte] = patch.read();
    }

    YM2420::write_patch(current_patch.ym2420_patch);
    current_patch.id = id;
  }
}
