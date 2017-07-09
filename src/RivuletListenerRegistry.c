#include "RivuletListenerRegistry.h"
#include "RivuletArray.h"

typedef struct RivuletListenerRegistration {
  rivulet_listener_next next;
  rivulet_listener_complete complete;
} RivuletListenerRegistration;

static RivuletListenerRegistration *
_create_registration (rivulet_listener_next next, rivulet_listener_complete complete) {
  RivuletListenerRegistration *registration = xmalloc (sizeof (RivuletListenerRegistration));
  registration->next = next;
  registration->complete = complete;
  return registration;
}

typedef struct RivuletListenerRegistry {
  RivuletArray *registrations;
} RivuletListenerRegistry;

static RivuletListenerRegistry *registry = NULL;

void rivulet_listener_registry_initialize () {
  if (registry != NULL) return;
  registry = xmalloc (sizeof (RivuletListenerRegistry));
  rivulet_array_initialize (&(registry->registrations));
}

RivuletListenerType rivulet_listener_registry_register (rivulet_listener_next next, rivulet_listener_complete complete) {
  if (registry == NULL) rivulet_listener_registry_initialize ();
  return (RivuletListenerType) (rivulet_array_push (registry->registrations, _create_registration (next, complete)) - 1);
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