#include "storage.h"
#include "hd44780.h"
#include "ym2420.h"
#include "menu.h"
#include "midi.h"
#include <Arduino.h>
#include <SD.h>

#define SDCARD_PRESENT 9
#define SDCARD_CS 8

#define PATCHES_FILE_PATH "YM2420.DAT"
#define PATCH_BYTES (PATCH_NAME_LENGTH + YM2420_PATCH_BYTES)

#define MIDI_SETTINGS_FILE_PATH "MIDI.DAT"

namespace Storage {
  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
    pinMode(SDCARD_CS, OUTPUT);

    Patches::current_id = 0;
    Patches::new_patch();

    MIDISettings::setup();
  }

  bool check_sdcard(bool show_error_message) {
    if (digitalRead(SDCARD_PRESENT)) {
      if (show_error_message) {
        HD44780::print_all("No SD card", "inserted!");
        Menu::set_expiration();
      }

      return false;
    }

    SD.begin(SDCARD_CS);
    return true;
  }

  namespace MIDISettings {
    uint8_t* midi_settings_file_format[MIDI_SETTINGS_FILE_BYTES] = {
        &MIDI::channel,

        &MIDI::CC::multi_sample_wave_carrier,
        &MIDI::CC::multi_sample_wave_modulation,
        &MIDI::CC::modulation_index,
        &MIDI::CC::fm_feedback_constant,
        &MIDI::CC::level_key_scale_carrier,
        &MIDI::CC::level_key_scale_modulation,

        &MIDI::CC::attack_rate_carrier,
        &MIDI::CC::decay_rate_carrier,
        &MIDI::CC::sustain_rate_carrier,
        &MIDI::CC::release_rate_carrier,
        &MIDI::CC::attack_rate_modulation,
        &MIDI::CC::decay_rate_modulation,
        &MIDI::CC::sustain_rate_modulation,
        &MIDI::CC::release_rate_modulation,

        &MIDI::CC::amplitude_modulation_carrier,
        &MIDI::CC::vibrato_carrier,
        &MIDI::CC::wave_distortion_carrier,
        &MIDI::CC::rate_key_scale_carrier,
        &MIDI::CC::amplitude_modulation_modulation,
        &MIDI::CC::vibrato_modulation,
        &MIDI::CC::wave_distortion_modulation,
        &MIDI::CC::rate_key_scale_modulation};

    void setup() {
      if (check_sdcard(false)) {
        File midi_settings_file = SD.open(MIDI_SETTINGS_FILE_PATH);
        bool midi_settings_file_found = (bool)midi_settings_file;

        if (midi_settings_file_found) {
          for (uint8_t byte = 0; byte < MIDI_SETTINGS_FILE_BYTES; byte++) {
            *midi_settings_file_format[byte] = midi_settings_file.read();
          }
        }

        midi_settings_file.close();

        if (! midi_settings_file_found) {
          MIDI::channel = MIDI_DEFAULT_CHANNEL;

          for (uint8_t byte = 1; byte < MIDI_SETTINGS_FILE_BYTES; byte++) {
            *midi_settings_file_format[byte] = byte + MIDI_DEFAULT_FIRST_CC - 1;
          }

          write();
        }
      }
    }

    void write() {
      if (check_sdcard(false)) {
        File midi_settings_file = SD.open(MIDI_SETTINGS_FILE_PATH, FILE_WRITE);
        midi_settings_file.seek(0);

        for (uint8_t byte = 0; byte < MIDI_SETTINGS_FILE_BYTES; byte++) {
          midi_settings_file.write(*midi_settings_file_format[byte]);
        }

        midi_settings_file.close();
      }
    }
  }

  namespace Patches {
    unsigned int current_id;
    char current_name[PATCH_NAME_LENGTH + 1];
    bool modified;

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

    void write(uint32_t id, char* name) {
      if (check_sdcard(true)) {
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
      if (check_sdcard(true)) {
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
}
