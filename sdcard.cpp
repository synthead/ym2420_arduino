#include "sdcard.h"
#include <Arduino.h>

#define SDCARD_PRESENT 9

namespace SDCard {
  void setup() {
    pinMode(SDCARD_PRESENT, INPUT);
  }

  bool present() {
    return ! digitalRead(SDCARD_PRESENT);
  }
}
