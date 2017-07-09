#include "RivuletListener.h"
#include "RivuletListenerRegistry.h"
#include "RivuletUtils.h"

RivuletListener *rivulet_listener_create (rivulet_listener_next next, rivulet_listener_complete complete) {
  RivuletListener *listener = xmalloc (sizeof (RivuletListener));
  listener->producer_type = 0;
  listener->listener_type = rivulet_listener_registry_register (next, complete);
  return listener;
}