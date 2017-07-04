#ifndef RIVULET_LAST_H
#define RIVULET_LAST_H

#include "RivuletOperator.h"

typedef struct RivuletLast {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_error _error;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  Boolean _has;
  int _value;
} RivuletLast;

RivuletLast *rivulet_last_create (RivuletStream *in);

#endif //RIVULET_LAST_H
