#include <pebble.h>
#include "front_menu.h" 

int main(void) {
  show_front_menu();
  app_event_loop();
  hide_front_menu();
}