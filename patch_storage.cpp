#include "patch_storage.h"
#include "hd44780.h"
#include "ym2420.h"
#include "menu.h"
#include <Arduino.h>
#include <SD.h>

#define SDCARD_PRESENT 9
#define SDCARD_CS 8

#define PATCHES_DIRECTORY "YM2420"
#define PATCH_NAMES_BYTES 17

namespace PatchStorage {
  struct current_patch_t {
    unsigned int id;
    const char* name;
  };

  current_patch_t current_patch = {
      .id = 0,
      .name = "Foobar town"
  };

  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
    pinMode(SDCARD_CS, OUTPUT);

    SD.begin(SDCARD_CS);
  }

  char print_patch() {
    char patch_number[4];
    sprintf(patch_number, "%03d", current_patch.id);
    HD44780::print_all(patch_number, current_patch.name);
  }

  void write_patch(unsigned int id, const char* name) {
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

    for (uint8_t byte = 0; byte < YM2420_PATCH_DATA_END; byte++) {
      patch.write(YM2420::register_contents[byte]);
    }

    patch.close();

    HD44780::print_all("Wrote patch", "to SD card.");
    Menu::set_temporary_message();
  }
}
