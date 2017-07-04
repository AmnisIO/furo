#ifndef RIVULET_LISTENER_H
#define RIVULET_LISTENER_H

#include "RivuletListenerInternal.h"

typedef struct RivuletListener {
  RivuletObservableType type;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_error _error;
  rivulet_listener_internal_complete _complete;
  void (*next) (struct RivuletListener *self, int v);
  void (*error) (struct RivuletListener *self, int e);
  void (*complete) (struct RivuletListener *self);
} RivuletListener;

typedef void (*rivulet_listener_next) (struct RivuletListener *self, int v);
typedef void (*rivulet_listener_error) (struct RivuletListener *self, int e);
typedef void (*rivulet_listener_complete) (struct RivuletListener *self);

RivuletListener *
rivulet_listener_create (rivulet_listener_next next, rivulet_listener_error error,
                         rivulet_listener_complete complete);
void rivulet_listener_initialize (RivuletListener *listener, rivulet_listener_next next,
                                  rivulet_listener_error error,
                                  rivulet_listener_complete complete);

#endif //RIVULET_LISTENER_H
