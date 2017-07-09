#ifndef RIVULET_PRODUCERPERIODIC_H
#define RIVULET_PRODUCERPERIODIC_H

#include "RivuletProducer.h"
#include "RivuletTimer.h"

typedef struct RivuletProducerPeriodic {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_producer_start start;
  rivulet_producer_stop stop;
  int _period;
  RivuletListener *_listener;
  RivuletTaskIdentifier _task_id;
} RivuletProducerPeriodic;

RivuletProducer *rivulet_producer_periodic_create (int period);

#endif //RIVULET_PRODUCERPERIODIC_H
