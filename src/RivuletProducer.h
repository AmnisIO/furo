#ifndef RIVULET_PRODUCER_H
#define RIVULET_PRODUCER_H

#include "RivuletListener.h"
#include "RivuletProducerInternal.h"

typedef struct RivuletProducer {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  void (*start) (struct RivuletProducer *self, struct RivuletListenerInternal *listener);
  void (*stop) (struct RivuletProducer *self);
} RivuletProducer;

typedef void (*rivulet_producer_start) (struct RivuletProducer *self,
                                        struct RivuletListenerInternal *listener);
typedef void (*rivulet_producer_stop) (struct RivuletProducer *self);

RivuletProducer *rivulet_producer_create (rivulet_producer_start start, rivulet_producer_stop stop);
void rivulet_producer_initialize (RivuletProducer *producer, rivulet_producer_start start,
                                  rivulet_producer_stop stop);

#endif // RIVULET_PRODUCER_H
