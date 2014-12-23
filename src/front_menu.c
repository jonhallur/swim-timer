#include <pebble.h>
#include "front_menu.h"
#include "swim_overview.h"
#include "swim.h"
#include "shared_constants.h"
  
#define NUM_SWIM_TYPES 4
#define NUM_POOL_LENGTH 2
#define NUM_MENU_SECTIONS 2 
#define NUM_FIRST_MENU_ITEMS 2
#define NUM_SECOND_MENU_ITEMS 2

const uint8_t pool_length[] = {25, 50};
const char *swim_type_name[] = { 
  "Front Crawl", 
  "Back Crawl", 
  "Breast Stroke", 
  "Butterfly Stroke" 
};

static uint8_t pool_length_index;
static uint8_t swim_type_index;

static char *menu_header[] = { 
  "Settings", 
  "Start",
};


// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static MenuLayer *s_front_menu_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  // s_front_menu_layer
  s_front_menu_layer = menu_layer_create(GRect(0, 0, 144, 152));
  menu_layer_set_click_config_onto_window(s_front_menu_layer, s_window);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_front_menu_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_front_menu_layer);
}
// END AUTO-GENERATED UI CODE

static void get_persisted_settings(void) {
  pool_length_index = persist_exists(POOL_LENGTH_PKEY) ? persist_read_int(POOL_LENGTH_PKEY) : 0;
  swim_type_index = persist_exists(SWIM_TYPE_PKEY) ? persist_read_int(SWIM_TYPE_PKEY) : 0;
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, menu_header[section_index]);
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  static char text[50];
  switch(cell_index->section) {
    case 0:
      switch(cell_index->row) {
        case 0:
          snprintf(text, sizeof("25 M"), "%d M", pool_length[pool_length_index]);
          menu_cell_basic_draw(ctx, cell_layer, "Pool length", text, NULL);
          break;
        case 1:
          strcpy(text, swim_type_name[swim_type_index]);
          menu_cell_basic_draw(ctx, cell_layer, "Swim type", text, NULL);
          break;
      }
      break;
    case 1:
      switch(cell_index->row) {
        case 0:
          menu_cell_title_draw(ctx, cell_layer, "Start Workout");
          break;
        case 1:
          menu_cell_title_draw(ctx, cell_layer, "View Last");
      }
  }
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          pool_length_index = (pool_length_index + 1) % NUM_POOL_LENGTH;
          layer_mark_dirty(menu_layer_get_layer(menu_layer));
          break;
        case 1:
          swim_type_index = (swim_type_index + 1) % NUM_SWIM_TYPES;
          layer_mark_dirty(menu_layer_get_layer(menu_layer));
          break;
      }
      break;
    case 1:
      switch(cell_index->row) {
        case 0:
          show_swim();
          break;
        case 1:
          show_swim_overview();
      }
  }
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}



void show_front_menu(void) {
  initialise_ui();
  get_persisted_settings();
  menu_layer_set_callbacks(s_front_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_front_menu(void) {
  window_stack_remove(s_window, true);
}
