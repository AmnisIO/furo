#ifndef RIVULET_PRODUCERPERIODIC_H
#define RIVULET_PRODUCERPERIODIC_H

#include "RivuletProducer.h"
#include "RivuletTimer.h"

typedef struct RivuletProducerPeriodic {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  int _period;
  RivuletListener *_listener;
  RivuletTaskIdentifier _task_id;
} RivuletProducerPeriodic;

RivuletProducer *rivulet_producer_periodic_create (int period);

#endif //RIVULET_PRODUCERPERIODIC_H
