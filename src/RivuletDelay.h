#ifndef RIVULET_DELAY_H
#define RIVULET_DELAY_H

#include "RivuletOperator.h"

typedef struct RivuletDelay {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int _delay;
} RivuletDelay;

RivuletProducer *byte_delay_create (RivuletStream *in, int delay);

#endif //RIVULET_DELAY_H
