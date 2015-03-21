#ifndef MENU_H
#define MENU_H

#include <stdbool.h>  // For bool.

struct temporary_message_t {
  unsigned long displayed_at;
  bool expired;
};

extern temporary_message_t temporary_message;

void startup_message();
void set_temporary_message();
void check_expired_message();

#endif
