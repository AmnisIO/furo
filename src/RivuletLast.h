#ifndef RIVULET_LAST_H
#define RIVULET_LAST_H

#include "RivuletOperator.h"

typedef struct RivuletLast {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  Boolean _has;
  int _value;
} RivuletLast;

RivuletProducer *rivulet_last_create (RivuletStream *in);

#endif //RIVULET_LAST_H
