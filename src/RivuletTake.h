#ifndef RIVULET_TAKE_H
#define RIVULET_TAKE_H


#include "RivuletOperator.h"

typedef struct RivuletTake {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int _to_take;
  int _taken;
} RivuletTake;

RivuletProducer *rivulet_take_create (RivuletStream *in, int count);

#endif //RIVULET_TAKE_H
