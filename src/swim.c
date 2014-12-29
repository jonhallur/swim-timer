#include <pebble.h>
#include "swim.h"
#include "swim_status.h"
#include "swim_overview.h"
#include "shared_constants.h"

static const char* c_state_text[] = {
  "Swim",
  "Rest",
  "Paused",
  "Start"
};
// Extra UI elements.
static GBitmap *s_res_image_action_stop;
static GBitmap *s_res_image_action_pause;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GBitmap *s_res_image_action_menu;
static GBitmap *s_res_image_action_play;
static GFont s_res_bitham_30_black;
static GFont s_res_droid_serif_28_bold;
static TextLayer *s_lap_timer_layer;
static ActionBarLayer *s_action_bar_layer;
static TextLayer *s_header_layer;
static TextLayer *s_distance_layer;
static TextLayer *s_total_timer_layer;
static InverterLayer *s_inverter_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_image_action_menu = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_MENU);
  s_res_image_action_play = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_PLAY);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  s_res_droid_serif_28_bold = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);
  // s_lap_timer_layer
  s_lap_timer_layer = text_layer_create(GRect(0, 34, 124, 49));
  text_layer_set_text(s_lap_timer_layer, "00:00");
  text_layer_set_font(s_lap_timer_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_lap_timer_layer);
  
  // s_action_bar_layer
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer, s_window);
  action_bar_layer_set_background_color(s_action_bar_layer, GColorBlack);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_res_image_action_menu);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_res_image_action_play);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_action_bar_layer);
  
  // s_header_layer
  s_header_layer = text_layer_create(GRect(0, 1, 124, 34));
  text_layer_set_text(s_header_layer, "Start?");
  text_layer_set_text_alignment(s_header_layer, GTextAlignmentCenter);
  text_layer_set_font(s_header_layer, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_header_layer);
  
  // s_distance_layer
  s_distance_layer = text_layer_create(GRect(10, 118, 104, 34));
  text_layer_set_text(s_distance_layer, "0m");
  text_layer_set_text_alignment(s_distance_layer, GTextAlignmentRight);
  text_layer_set_font(s_distance_layer, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_distance_layer);
  
  // s_total_timer_layer
  s_total_timer_layer = text_layer_create(GRect(0, 84, 124, 34));
  text_layer_set_text(s_total_timer_layer, "00:00:00");
  text_layer_set_text_alignment(s_total_timer_layer, GTextAlignmentCenter);
  text_layer_set_font(s_total_timer_layer, s_res_droid_serif_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_total_timer_layer);
  
  // s_inverter_layer
  s_inverter_layer = inverter_layer_create(GRect(0, 0, 145, 153));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverter_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_lap_timer_layer);
  action_bar_layer_destroy(s_action_bar_layer);
  text_layer_destroy(s_header_layer);
  text_layer_destroy(s_distance_layer);
  text_layer_destroy(s_total_timer_layer);
  inverter_layer_destroy(s_inverter_layer);
  gbitmap_destroy(s_res_image_action_menu);
  gbitmap_destroy(s_res_image_action_play);
}
// END AUTO-GENERATED UI CODE

static void save_current_status(void) {
  persist_write_int(SWIM_STATUS_TOTAL_TIME_PKEY, (int)timer_get_total_time());
  persist_write_int(SWIM_STATUS_REST_TIME_PKEY, (int)timer_get_total_rest_time());
  persist_write_int(SWIM_STATUS_SWIM_TIME_PKEY, (int)timer_get_total_swim_time());
  persist_write_int(SWIM_STATUS_DISTANCE_PKEY, (int)timer_get_total_distance());
  persist_write_int(SWIM_STATUS_LAP_COUNT_PKEY, (int)timer_get_total_laps());
}

static void initialise_extra(void) {
  s_res_image_action_stop = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_END);
  s_res_image_action_pause = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_PAUSE);
  layer_remove_from_parent((Layer *)s_inverter_layer);
}

static void destroy_extra(void) {
  gbitmap_destroy(s_res_image_action_stop);
  gbitmap_destroy(s_res_image_action_pause);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  TimerState state = timer_get_state();
  switch(state) {
    case SWIM_TIMER_PAUSED:
      timer_lap_resume();
      break;
    case SWIM_TIMER_RESTING:
      timer_lap_start();
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_res_image_action_stop);
      layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverter_layer);
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_res_image_action_pause);
      vibes_short_pulse();
      break;
    case SWIM_TIMER_SWIMMING:
      timer_lap_end();
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_res_image_action_play);
      layer_remove_from_parent((Layer *)s_inverter_layer);
      action_bar_layer_clear_icon(s_action_bar_layer, BUTTON_ID_DOWN);
      vibes_short_pulse();
      break;
    case SWIM_TIMER_NOT_STARTED:
      timer_lap_start();
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_res_image_action_stop);
      layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverter_layer);
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_res_image_action_pause);
      vibes_short_pulse();
      break;
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  TimerState state = timer_get_state();
  switch (state) {
    case SWIM_TIMER_PAUSED:
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_res_image_action_pause);
      timer_lap_resume();
      vibes_short_pulse();
      break;
    case SWIM_TIMER_RESTING:
      break;
    case SWIM_TIMER_SWIMMING:
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_res_image_action_play);
      timer_lap_pause();
      vibes_short_pulse();
      break;
    case SWIM_TIMER_NOT_STARTED:
      break;
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  save_current_status();
  show_swim_overview();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) up_click_handler);
}

static void setup_action_bar() {
  action_bar_layer_set_click_config_provider(s_action_bar_layer, click_config_provider);
}

static void update_state_text(TimerState state) {
    text_layer_set_text(s_header_layer, c_state_text[state]);
}

static void update_lap_timer_text(uint16_t time) {
  static char timer_text[10];
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(timer_text), "%.02d:%.02d", minute, second);
  text_layer_set_text(s_lap_timer_layer, timer_text);
}

static void update_total_timer_text(uint16_t time) {
  static char timer_text[10];
  uint hour = time / SECONDS_IN_HOUR;
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(timer_text), "%.02d:%.02d:%.02d", hour, minute, second);
  text_layer_set_text(s_total_timer_layer, timer_text);
}

static void update_distance_text(uint16_t distance) {
  static char distance_text[10];
  snprintf(distance_text, sizeof(distance_text), "%dm", distance);
  text_layer_set_text(s_distance_layer, distance_text);
}

static void initialize_ui_elements() {
  update_state_text(timer_get_state());
  update_lap_timer_text(timer_get_lap_time());
  update_total_timer_text(timer_get_total_time());
  update_distance_text(timer_get_total_distance());
}

static void handle_window_unload(Window* window) {
  save_current_status();
  destroy_ui();
  destroy_extra();
}

void show_swim(void) {
  initialise_ui();
  initialise_extra();
  timer_set_pool_length();
  timer_set_callbacks((TimerHandlers) {
    .state = &update_state_text,
    .lap = &update_lap_timer_text,
    .total = &update_total_timer_text,
    .distance = &update_distance_text
  });
  initialize_ui_elements();
  setup_action_bar();
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_swim(void) {
  window_stack_remove(s_window, true);
}
