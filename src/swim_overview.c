#include "swim_overview.h"
#include <pebble.h>
#include "shared_constants.h"

static const unsigned int SECONDS_IN_MINUTE = 60;
static const unsigned int SECONDS_IN_HOUR = 3600;

static uint page_index = 0;

static char total_time_text[10];
static char rest_time_text[10];
static char swim_time_text[10];
static char distance_text[10];
static char average_text[10];
static char lap_count_text[10];
static char front_crawl_distance_text[10];
static char back_crawl_distance_text[10];
static char breast_stroke_distance_text[10];

static char* label_1[3] = {"Total time", "Average lap time", "Front Crawl"};
static char* label_2[3] = {"Swim time", "Total Distance", "Back Crawl"};
static char* label_3[3] = {"Rest time", "Lap count", "Breast Strokes"};
  
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_droid_serif_28_bold;
static TextLayer *s_label_layer_1;
static TextLayer *s_text_layer_1;
static TextLayer *s_label_layer_2;
static TextLayer *s_text_layer_2;
static TextLayer *s_label_layer_3;
static TextLayer *s_text_layer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 0);
  
  s_res_droid_serif_28_bold = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);
  // s_label_layer_1
  s_label_layer_1 = text_layer_create(GRect(0, 0, 144, 16));
  text_layer_set_text(s_label_layer_1, "label_1");
  text_layer_set_text_alignment(s_label_layer_1, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_layer_1);
  
  // s_text_layer_1
  s_text_layer_1 = text_layer_create(GRect(0, 15, 144, 30));
  text_layer_set_text(s_text_layer_1, "00:00:00");
  text_layer_set_text_alignment(s_text_layer_1, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer_1, s_res_droid_serif_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_text_layer_1);
  
  // s_label_layer_2
  s_label_layer_2 = text_layer_create(GRect(0, 50, 144, 16));
  text_layer_set_text(s_label_layer_2, "label_2");
  text_layer_set_text_alignment(s_label_layer_2, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_layer_2);
  
  // s_text_layer_2
  s_text_layer_2 = text_layer_create(GRect(0, 65, 144, 30));
  text_layer_set_text(s_text_layer_2, "00:00:00");
  text_layer_set_text_alignment(s_text_layer_2, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer_2, s_res_droid_serif_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_text_layer_2);
  
  // s_label_layer_3
  s_label_layer_3 = text_layer_create(GRect(0, 100, 144, 16));
  text_layer_set_text(s_label_layer_3, "label_3");
  text_layer_set_text_alignment(s_label_layer_3, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_label_layer_3);
  
  // s_text_layer_3
  s_text_layer_3 = text_layer_create(GRect(0, 115, 144, 30));
  text_layer_set_text(s_text_layer_3, "00:00:00");
  text_layer_set_text_alignment(s_text_layer_3, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer_3, s_res_droid_serif_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_text_layer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_label_layer_1);
  text_layer_destroy(s_text_layer_1);
  text_layer_destroy(s_label_layer_2);
  text_layer_destroy(s_text_layer_2);
  text_layer_destroy(s_label_layer_3);
  text_layer_destroy(s_text_layer_3);
}
// END AUTO-GENERATED UI CODE

void format_big_time(char *timer_text, uint16_t time) {
  uint hour = time / SECONDS_IN_HOUR;
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(char)*10, "%.02d:%.02d:%.02d", hour, minute, second);
}

void format_small_time(char *timer_text, uint16_t time) {
  uint minute = (time % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; 
  uint second = time % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(char)*10, "%.02d:%.02d", minute, second);
}

void format_time(char *timer_text, uint16_t time) {
  time > SECONDS_IN_HOUR ? format_big_time(timer_text, time) : format_small_time(timer_text, time);
}

static void set_text_elements(int index) {
  text_layer_set_text(s_label_layer_1, label_1[index]);
  text_layer_set_text(s_label_layer_2, label_2[index]);
  text_layer_set_text(s_label_layer_3, label_3[index]);
  if (index == 0) {
    text_layer_set_text(s_text_layer_1, total_time_text);
    text_layer_set_text(s_text_layer_2, swim_time_text);
    text_layer_set_text(s_text_layer_3, rest_time_text);
  }
  else if (index == 1) {
    text_layer_set_text(s_text_layer_1, average_text);
    text_layer_set_text(s_text_layer_2, distance_text);
    text_layer_set_text(s_text_layer_3, lap_count_text);
  }
  else if (index == 2) {
    text_layer_set_text(s_text_layer_1, front_crawl_distance_text);
    text_layer_set_text(s_text_layer_2, back_crawl_distance_text);
    text_layer_set_text(s_text_layer_3, breast_stroke_distance_text);
  }
}

static void initialise_text_elements(void) {
  int total_time = persist_read_int(SWIM_STATUS_TOTAL_TIME_PKEY);
  int rest_time = persist_read_int(SWIM_STATUS_REST_TIME_PKEY);
  int swim_time = persist_read_int(SWIM_STATUS_SWIM_TIME_PKEY);
  int distance = persist_read_int(SWIM_STATUS_DISTANCE_PKEY);
  int lap_count = persist_read_int(SWIM_STATUS_LAP_COUNT_PKEY);
  int front_dist = persist_read_int(SWIM_STATUS_FRONT_CRAWL_DISTANCE_PKEY);
  int back_dist = persist_read_int(SWIM_STATUS_BACK_CRAWL_DISTANCE_PKEY);
  int breast_dist  = persist_read_int(SWIM_STATUS_BREAST_STROKE_DISTANCE_PKEY);
  
  format_time(total_time_text, total_time);
  format_time(rest_time_text, rest_time);
  format_time(swim_time_text, swim_time);
  format_time(average_text, (swim_time / lap_count));
  snprintf(distance_text, sizeof(distance_text), "%dm", distance);
  snprintf(lap_count_text, sizeof(lap_count_text), "%d", lap_count);
  snprintf(front_crawl_distance_text, sizeof(front_crawl_distance_text), "%dm", front_dist);
  snprintf(back_crawl_distance_text, sizeof(back_crawl_distance_text), "%dm", back_dist);
  snprintf(breast_stroke_distance_text, sizeof(breast_stroke_distance_text), "%dm", breast_dist);

  set_text_elements(page_index);
}

static void page_click_handler(ClickRecognizerRef recognizer, void *context) {
  page_index = (page_index + 1) % 3;
  set_text_elements(page_index);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
   hide_swim_overview();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) page_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) page_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_swim_overview(void) {
  initialise_ui();
  initialise_text_elements();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_swim_overview(void) {
  window_stack_remove(s_window, true);
}
