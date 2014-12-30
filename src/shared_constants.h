#ifndef _SHARED_CONSTANTS_H
#define _SHARED_CONSTANTS_H
#include <pebble.h>
#define POOL_LENGTH_PKEY 0
#define SWIM_TYPE_PKEY 1
#define SWIM_STATUS_TOTAL_TIME_PKEY 2
#define SWIM_STATUS_DISTANCE_PKEY 3
#define SWIM_STATUS_SWIM_TIME_PKEY 4
#define SWIM_STATUS_REST_TIME_PKEY 5
#define SWIM_STATUS_LAP_COUNT_PKEY 6
#define SWIM_STATUS_FRONT_CRAWL_DISTANCE_PKEY 7
#define SWIM_STATUS_BACK_CRAWL_DISTANCE_PKEY 8
#define SWIM_STATUS_BREAST_STROKE_DISTANCE_PKEY 9
extern const uint8_t pool_length[];
typedef enum { FRONT_CRAWL, BACK_CRAWL, BREAST_STROKE } swim_types;
extern const char *swim_type_name[];
#endif //_CONSTANTS_H_