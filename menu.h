#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

namespace Menu {
  struct temporary_message_t {
    unsigned long displayed_at;
    bool expired;
    bool menu_active;
  };

  extern temporary_message_t temporary_message;

  void startup_message();
  void set_temporary_message();
  void check_expired_message();
  void scan_inputs();
}

#endif
