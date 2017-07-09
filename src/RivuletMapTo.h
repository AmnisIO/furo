#ifndef RIVULET_MAPTO_H
#define RIVULET_MAPTO_H

#include "RivuletOperator.h"

typedef struct RivuletMapTo {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int value;
} RivuletMapTo;

RivuletProducer *rivulet_map_to_create (RivuletStream *in, int value);

#endif //RIVULET_MAPTO_H
