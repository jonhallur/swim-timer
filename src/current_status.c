#include "current_status.h"
#include "shared_constants.h"
#include <pebble.h>

static const unsigned int SECONDS_IN_MINUTE = 60;
static const unsigned int SECONDS_IN_HOUR = 3600;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_24_bold;
static TextLayer *s_labels_text_layer;
static TextLayer *s_times_text_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // s_labels_text_layer
  s_labels_text_layer = text_layer_create(GRect(0, 0, 72, 168));
  text_layer_set_text(s_labels_text_layer, "Text layer");
  text_layer_set_font(s_labels_text_layer, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_labels_text_layer);
  
  // s_times_text_layer
  s_times_text_layer = text_layer_create(GRect(72, 0, 72, 168));
  text_layer_set_text(s_times_text_layer, "00:00:00");
  text_layer_set_text_alignment(s_times_text_layer, GTextAlignmentRight);
  text_layer_set_font(s_times_text_layer, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_times_text_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_labels_text_layer);
  text_layer_destroy(s_times_text_layer);
}
// END AUTO-GENERATED UI CODE

void format_big_timer(char *timer_text, uint16_t time) {
  uint hour = time / SECONDS_IN_HOUR;
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(char)*10, "%.02d:%.02d:%.02d", hour, minute, second);
}

void format_small_timer(char *timer_text, uint16_t time) {
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(char)*10, "%.02d:%.02d", minute, second);
}

void format_timer(char *timer_text, uint16_t time) {
  time > SECONDS_IN_HOUR ? format_big_timer(timer_text, time) : format_small_timer(timer_text, time);
}

static void set_current_status_text(void) {
  static char total_time_text[10];
  static char rest_time_text[10];
  static char swim_time_text[10];
  static char distance_text[10];
  static char average_text[10];
  int total_time = persist_read_int(SWIM_STATUS_TOTAL_TIME_PKEY);
  int rest_time = persist_read_int(SWIM_STATUS_REST_TIME_PKEY);
  int swim_time = persist_read_int(SWIM_STATUS_SWIM_TIME_PKEY);
  int distance = persist_read_int(SWIM_STATUS_DISTANCE_PKEY);
  int lap_count = persist_read_int(SWIM_STATUS_LAP_COUNT_PKEY);
  static char text[50];
  format_timer(total_time_text, total_time);
  format_timer(rest_time_text, rest_time);
  format_timer(swim_time_text, swim_time);
  format_timer(average_text, (swim_time / lap_count));
  snprintf(distance_text, sizeof(distance_text), "%dm", distance);
  snprintf(text, sizeof(text), "%s\n%s\n%s\n%s\n%s\n%d", total_time_text, swim_time_text, rest_time_text, average_text, distance_text, lap_count);
  text_layer_set_text(s_times_text_layer, text);
  text_layer_set_text(s_labels_text_layer, "Total:\nSwim:\nRest:\nAvrg:\nDist:\nLaps:");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  hide_current_status(); 
}
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_current_status(void) {
  initialise_ui();
  set_current_status_text();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_current_status(void) {
  window_stack_remove(s_window, true);
}
