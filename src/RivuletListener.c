#include "RivuletListener.h"

RivuletListener *rivulet_listener_create () {
  RivuletListener *listener = xmalloc (sizeof (RivuletListener));
  listener->producer_type = 0;
  listener->listener_type = rivulet_listener_registry_register (_next, _complete);
  return listener;
}