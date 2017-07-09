#ifndef RIVULET_RIVULETPRODUCERREGISTRY_H
#define RIVULET_RIVULETPRODUCERREGISTRY_H

#include "RivuletProducer.h"

typedef struct RivuletProducerRegistration {
  rivulet_producer_start start;
  rivulet_producer_stop stop;
} RivuletProducerRegistration;

RivuletProducerType rivulet_producer_registry_register (RivuletProducerRegistration *);
rivulet_producer_start rivulet_producer_registry_get_start (RivuletProducerType);
rivulet_producer_stop rivulet_producer_registry_get_stop (RivuletProducerType);

#endif //RIVULET_RIVULETPRODUCERREGISTRY_H
