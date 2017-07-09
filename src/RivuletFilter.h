#ifndef RIVULET_FILTER_H
#define RIVULET_FILTER_H

#include "RivuletOperator.h"

typedef Boolean (*rivulet_filter_function) (int value);

typedef struct RivuletFilter {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  rivulet_filter_function filter;
} RivuletFilter;

RivuletProducer *rivulet_filter_create (RivuletStream *in, rivulet_filter_function filter);

#endif //RIVULET_FILTER_H
