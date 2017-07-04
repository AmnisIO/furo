#ifndef RIVULET_MAPTO_H
#define RIVULET_MAPTO_H

#include "RivuletOperator.h"

typedef struct RivuletMapTo {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_error _error;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  int value;
} RivuletMapTo;

RivuletMapTo *rivulet_map_to_create (RivuletStream *in, int value);

#endif //RIVULET_MAPTO_H
