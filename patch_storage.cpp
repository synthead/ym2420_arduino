#include "patch_storage.h"
#include "hd44780.h"
#include "ym2420.h"
#include "menu.h"
#include <Arduino.h>
#include <SD.h>

#define SDCARD_PRESENT 9
#define SDCARD_CS 8

#define PATCHES_FILE_PATH "YM2420.DAT"
#define PATCH_BYTES (PATCH_NAME_LENGTH + YM2420_PATCH_BYTES)

namespace PatchStorage {
  unsigned int current_id;
  char current_name[PATCH_NAME_LENGTH + 1];
  bool modified;

  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
    pinMode(SDCARD_CS, OUTPUT);

    current_id = 0;
    new_patch();
  }

  void new_patch() {
    modified = true;
    memset(current_name, ' ', PATCH_NAME_LENGTH);
    current_name[PATCH_NAME_LENGTH] = NULL;

    print_patch();
  }

  void print_patch() {
    if (modified) {
      HD44780::print_all("Unsaved patch", "");
    } else {
      char patch_number[17];
      sprintf(patch_number, "Patch %d:", current_id);
      HD44780::print_all(patch_number, current_name);
    }
  }

  bool check_sdcard() {
    if (digitalRead(SDCARD_PRESENT)) {
      HD44780::print_all("No SD card", "inserted!");
      Menu::set_expiration();
      return false;
    }

    SD.begin(SDCARD_CS);
    return true;
  }

  void write(uint32_t id, char* name) {
    if (check_sdcard()) {
      File patches_file = SD.open(PATCHES_FILE_PATH, FILE_WRITE);
      if (id == FIND_NEXT_ID) {
        id = patches_file.size() / PATCH_BYTES;
      }
      patches_file.seek(PATCH_BYTES * id);

      patches_file.write(name);

      for (uint8_t byte = 0; byte < YM2420_PATCH_BYTES; byte++) {
        patches_file.write(YM2420::register_contents[byte]);
      }

      patches_file.close();

      modified = false;
      current_id = id;
      strcpy(current_name, name);
      print_patch();
    }
  }

  void read(uint32_t id) {
    if (check_sdcard()) {
      File patches_file = SD.open(PATCHES_FILE_PATH);

      if (! patches_file) {
        HD44780::print_all("No patches exist", "on SD card!");
        Menu::set_expiration();
      } else if (patches_file.size() / PATCH_BYTES > id) {
        patches_file.seek(PATCH_BYTES * id);

        for (uint8_t byte = 0; byte < PATCH_NAME_LENGTH; byte++) {
          current_name[byte] = patches_file.read();
        }

        for (uint8_t byte = 0; byte < YM2420_PATCH_BYTES; byte++) {
          YM2420::register_contents[byte] = patches_file.read();
          YM2420::write(byte);
        }

        modified = false;
        current_id = id;
        print_patch();
      }

      patches_file.close();
    }
  }

  void read_next() {
    if (modified) {
      read(current_id);
    } else {
      read(current_id + 1);
    }
  }

  void read_previous() {
    if (modified) {
      read(current_id);
    } else {
      if (current_id != 0) {
        read(current_id - 1);
      }
    }
  }
}
