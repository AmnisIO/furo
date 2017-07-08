#ifndef RIVULET_MAP_H
#define RIVULET_MAP_H

#include "RivuletOperator.h"

typedef int (*rivulet_map_function) (int value);

typedef struct RivuletMap {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  rivulet_map_function map;
} RivuletMap;

RivuletMap *rivulet_map_create (RivuletStream *in, rivulet_map_function map);

#endif //RIVULET_MAP_H
