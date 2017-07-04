#ifndef RIVULET_FILTER_H
#define RIVULET_FILTER_H

#include "RivuletOperator.h"

typedef Boolean (*rivulet_filter_function) (int value);

typedef struct RivuletFilter {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_error _error;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  rivulet_filter_function filter;
} RivuletFilter;

RivuletFilter *rivulet_filter_create (RivuletStream *in, rivulet_filter_function filter);

#endif //RIVULET_FILTER_H
