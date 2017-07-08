#ifndef RIVULET_DELAY_H
#define RIVULET_DELAY_H

#include "RivuletOperator.h"

typedef struct RivuletDelay {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  int _delay;
} RivuletDelay;

RivuletDelay *byte_delay_create (RivuletStream *in, int delay);

#endif //RIVULET_DELAY_H
