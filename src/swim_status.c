#include <pebble.h>
#include "swim_status.h"
#include "shared_constants.h"

// states
static TimerState m_current_state = SWIM_TIMER_NOT_STARTED;
static uint16_t m_start_time;
static uint16_t m_total_time;
static uint16_t m_total_swim_time;
static uint16_t m_current_lap_time;
static uint16_t m_current_rest_time;
static uint16_t m_current_distance;
static uint16_t m_current_lap_count;
static uint8_t m_pool_length;
static uint8_t m_swim_type;
static TimerHandlers p_callbacks;

void setup_timer() {
  m_start_time = time(NULL);
  m_current_state = SWIM_TIMER_SWIMMING;
  uint8_t pool_length_index = persist_exists(POOL_LENGTH_PKEY) ? persist_read_int(POOL_LENGTH_PKEY) : 0;
  m_pool_length = pool_length[pool_length_index];
  p_callbacks.state(m_current_state);
  p_callbacks.lap(0);
  p_callbacks.total(0);
  p_callbacks.distance(0);
}
  
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  m_total_time++;
  switch(m_current_state) {
    case SWIM_TIMER_PAUSED:
      break;
    case SWIM_TIMER_RESTING:
      m_current_rest_time++;
      break;
    case SWIM_TIMER_SWIMMING:
      m_total_swim_time++;
      m_current_lap_time++;
      p_callbacks.lap(m_current_lap_time);
      break;
    case SWIM_TIMER_NOT_STARTED:
    default:
      break;
  }
  p_callbacks.total(m_total_time);
}
  
void timer_subscribe() {
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}


void timer_unsubscribe() {
  tick_timer_service_unsubscribe();
}

void timer_set_callbacks(TimerHandlers callbacks) {
  p_callbacks.total = callbacks.total;
  p_callbacks.lap = callbacks.lap;
  p_callbacks.state = callbacks.state;
  p_callbacks.distance = callbacks.distance;
}

void timer_lap_start() {
  if (m_start_time == 0 || m_current_state == SWIM_TIMER_NOT_STARTED) {
    setup_timer();
    timer_subscribe();
  }
  m_current_state = SWIM_TIMER_SWIMMING;
  m_current_lap_time = 0;
  p_callbacks.state(m_current_state);
  p_callbacks.lap(m_current_lap_time);
}

void timer_lap_end() {
  m_current_state = SWIM_TIMER_RESTING;
  m_current_rest_time = 0;
  m_current_lap_count++;
  m_current_distance += m_pool_length;
  p_callbacks.state(m_current_state);
  p_callbacks.distance(m_current_distance);
}

void timer_lap_pause() {
  m_current_state = SWIM_TIMER_PAUSED;
  p_callbacks.state(m_current_state);
}

void timer_lap_resume() {
  m_current_state = SWIM_TIMER_SWIMMING;
  p_callbacks.state(m_current_state);
}

bool timer_is_running() {
  if ((m_current_state == SWIM_TIMER_NOT_STARTED) || (m_current_state == SWIM_TIMER_PAUSED)) {
    return false;
  }
  return true;
}

//Timer setters
void timer_set_pool_length() {
  uint8_t pool_length_index = persist_exists(POOL_LENGTH_PKEY) ? persist_read_int(POOL_LENGTH_PKEY) : 0;
  m_pool_length = pool_length[pool_length_index];
}


void timer_set_swim_type() {
  m_swim_type = persist_exists(SWIM_TYPE_PKEY) ? persist_read_int(SWIM_TYPE_PKEY) : 0;
}

//Timer getters
uint16_t timer_get_lap_time() {
  return m_current_lap_time;
}

uint16_t timer_get_rest_time() {
  return m_current_rest_time;
}

uint16_t timer_get_total_rest_time() {
  return m_total_time - m_total_swim_time;
}
uint16_t timer_get_total_time() {
  return m_total_time;
}

uint16_t timer_get_total_swim_time() {
  return m_total_swim_time;
}

uint16_t timer_get_total_distance() {
  return m_current_distance;
}

uint16_t timer_get_total_laps() {
  return m_current_lap_count;
}

TimerState timer_get_state() {
  return m_current_state;
}
