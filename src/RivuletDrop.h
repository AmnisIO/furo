#ifndef RIVULET_DROP_H
#define RIVULET_DROP_H

#include "RivuletOperator.h"

typedef struct RivuletDrop {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  int _to_drop;
  int _dropped;
} RivuletDrop;

RivuletDrop *rivulet_drop_create (RivuletStream *in, int count);

#endif //RIVULET_DROP_H
