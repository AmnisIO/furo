#ifndef RIVULET_LISTENER_H
#define RIVULET_LISTENER_H

#include "RivuletListenerType.h"
#include "RivuletProducerType.h"

typedef struct RivuletListener {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
} RivuletListener;

typedef void (*rivulet_listener_next) (RivuletListener *, int);
typedef void (*rivulet_listener_complete) (RivuletListener *);

#endif //RIVULET_LISTENER_H
