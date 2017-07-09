#ifndef RIVULET_PRODUCER_H
#define RIVULET_PRODUCER_H

#include "RivuletListener.h"

typedef struct RivuletProducer {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
} RivuletProducer;

typedef void (*rivulet_producer_start) (RivuletProducer *, RivuletListener *);
typedef void (*rivulet_producer_stop) (RivuletProducer *);

#endif // RIVULET_PRODUCER_H
