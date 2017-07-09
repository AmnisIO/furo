#ifndef RIVULET_OBSERVABLE_H
#define RIVULET_OBSERVABLE_H

#include "RivuletListenerType.h"
#include "RivuletProducerType.h"

typedef struct RivuletObservable {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
} RivuletObservable;

#endif //RIVULET_OBSERVABLE_H
