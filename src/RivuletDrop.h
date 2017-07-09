#ifndef RIVULET_DROP_H
#define RIVULET_DROP_H

#include "RivuletOperator.h"

typedef struct RivuletDrop {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int _to_drop;
  int _dropped;
} RivuletDrop;

RivuletProducer *rivulet_drop_create (RivuletStream *in, int count);

#endif //RIVULET_DROP_H
