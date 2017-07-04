#ifndef RIVULET_TAKE_H
#define RIVULET_TAKE_H


#include "RivuletOperator.h"

typedef struct RivuletTake {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_error _error;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  int _to_take;
  int _taken;
} RivuletTake;

RivuletTake *rivulet_take_create (RivuletStream *in, int count);

#endif //RIVULET_TAKE_H
