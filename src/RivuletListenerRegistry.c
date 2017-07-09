#include "RivuletListenerRegistry.h"
#include "RivuletArray.h"

typedef struct RivuletListenerRegistry {
  RivuletArray *registrations;
} RivuletListenerRegistry;

static RivuletListenerRegistry *registry = NULL;

void rivulet_listener_registry_initialize () {
  if (registry != NULL) return;
  registry = xmalloc (sizeof (RivuletListenerRegistry));
  rivulet_array_initialize (&(registry->registrations));
}

RivuletListenerType rivulet_listener_registry_register (RivuletListenerRegistration *registration) {
  if (registry == NULL) rivulet_listener_registry_initialize ();
  return (RivuletListenerType) rivulet_array_push (registry->registrations, registration);
}

RivuletListenerRegistration *rivulet_listener_registry_get (RivuletListenerType type) {
  return (RivuletListenerRegistration *) rivulet_array_get (registry->registrations, type);
}

rivulet_listener_next rivulet_listener_registry_get_next (RivuletListenerType type) {
  return rivulet_listener_registry_get (type)->next;
}

rivulet_listener_complete rivulet_listener_registry_get_complete (RivuletListenerType type) {
  return rivulet_listener_registry_get (type)->complete;
}