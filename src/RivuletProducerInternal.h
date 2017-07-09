#ifndef RIVULET_PRODUCERINTERNAL_H
#define RIVULET_PRODUCERINTERNAL_H

#include "RivuletListener.h"
#include "RivuletObservable.h"

typedef struct RivuletProducerInternal {
  RivuletObservableType type;
  void (*_start) (struct RivuletProducerInternal *self, struct RivuletListenerInternal *listener);
  void (*_stop) (struct RivuletProducerInternal *self);
} RivuletProducerInternal;

typedef void (*rivulet_producer_internal_start) (struct RivuletProducerInternal *self,
                                                 struct RivuletListenerInternal *listener);
typedef void (*rivulet_producer_internal_stop) (struct RivuletProducerInternal *self);

#endif // RIVULET_PRODUCERINTERNAL_H
