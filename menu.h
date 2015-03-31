#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdint.h>

namespace Menu {
  extern uint8_t active_menu;

  void startup_message();
  void set_expiration();
  void check_expired_message();
  void scan_inputs();
}

#endif
