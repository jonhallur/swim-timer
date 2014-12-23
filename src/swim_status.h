#pragma once
#include <pebble.h>
  
typedef enum {
  SWIM_TIMER_SWIMMING,
  SWIM_TIMER_RESTING,
  SWIM_TIMER_PAUSED,
  SWIM_TIMER_NOT_STARTED
} TimerState;

typedef void (*update_activity_state)(TimerState);
typedef void (*update_lap_counter)(uint16_t);
typedef void (*update_total_timer)(uint16_t);
typedef void (*update_distance)(uint16_t);

typedef struct TimerHandlers {
  update_activity_state state;
  update_lap_counter lap;
  update_total_timer total;
  update_distance distance;
} TimerHandlers;

//Functions
void timer_subscribe();
void timer_unsubscribe();
void timer_set_callbacks(TimerHandlers handlers);
void timer_lap_start();
void timer_lap_end();
void timer_lap_pause();
void timer_lap_resume();

bool timer_is_running();
//Timer getters
uint16_t timer_get_lap_time();
uint16_t timer_get_rest_time();
uint16_t timer_get_total_rest_time();
uint16_t timer_get_total_time();
uint16_t timer_get_total_swim_time();
uint16_t timer_get_total_distance();
uint16_t timer_get_total_laps();
TimerState timer_get_state();
