#ifndef RIVULET_MAP_H
#define RIVULET_MAP_H

#include "RivuletOperator.h"

typedef int (*rivulet_map_function) (int value);

typedef struct RivuletMap {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  rivulet_map_function map;
} RivuletMap;

RivuletProducer *rivulet_map_create (RivuletStream *, rivulet_map_function);

#endif //RIVULET_MAP_H
