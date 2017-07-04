#include "RivuletListener.h"

static void _next (struct RivuletListenerInternal *self, int v) {
  RivuletListener *listener = (RivuletListener *) self;
  listener->next (listener, v);
}
static void _error (struct RivuletListenerInternal *self, int e) {
  RivuletListener *listener = (RivuletListener *) self;
  listener->error (listener, e);
}
static void _complete (struct RivuletListenerInternal *self) {
  RivuletListener *listener = (RivuletListener *) self;
  listener->complete (listener);
}

RivuletListener *
rivulet_listener_create (rivulet_listener_next next, rivulet_listener_error error,
                         rivulet_listener_complete complete) {
  RivuletListener *listener = xmalloc (sizeof (RivuletListener));
  rivulet_listener_initialize (listener, next, error, complete);
  return listener;
}

void rivulet_listener_initialize (RivuletListener *listener, rivulet_listener_next next,
                                  rivulet_listener_error error,
                                  rivulet_listener_complete complete) {
  listener->type = RIVULET_OBSERVABLE_TYPE_LISTENER;
  listener->_next = _next;
  listener->_error = _error;
  listener->_complete = _complete;
  listener->next = next;
  listener->error = error;
  listener->complete = complete;
}