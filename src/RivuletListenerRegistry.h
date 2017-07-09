#ifndef RIVULET_RIVULETLISTENERREGISTRY_H
#define RIVULET_RIVULETLISTENERREGISTRY_H

#include "RivuletListener.h"
#include "RivuletArray.h"
#include "RivuletListenerType.h"

typedef struct RivuletListenerRegistration {
  rivulet_listener_next next;
  rivulet_listener_complete complete;
} RivuletListenerRegistration;

RivuletListenerType rivulet_listener_registry_register (RivuletListenerRegistration *);
rivulet_listener_next rivulet_listener_registry_get_next (RivuletListenerType);
rivulet_listener_complete rivulet_listener_registry_get_complete (RivuletListenerType);

#endif //RIVULET_RIVULETLISTENERREGISTRY_H
