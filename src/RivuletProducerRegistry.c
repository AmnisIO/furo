#include "RivuletProducerRegistry.h"
#include "RivuletArray.h"

typedef struct RivuletProducerRegistry {
  RivuletArray *registrations;
} RivuletProducerRegistry;

static RivuletProducerRegistry *registry = NULL;

void rivulet_producer_registry_initialize () {
  if (registry != NULL) return;
  registry = xmalloc (sizeof (RivuletProducerRegistry));
  rivulet_array_initialize (&(registry->registrations));
}

RivuletProducerType rivulet_producer_registry_register (RivuletProducerRegistration *registration) {
  if (registry == NULL) rivulet_producer_registry_initialize ();
  return (RivuletProducerType) rivulet_array_push (registry->registrations, registration);
}

RivuletProducerRegistration *rivulet_producer_registry_get (RivuletProducerType type) {
  return (RivuletProducerRegistration *) rivulet_array_get (registry->registrations, type);
}

rivulet_producer_start rivulet_producer_registry_get_start (RivuletProducerType type) {
  return rivulet_producer_registry_get (type)->start;
}

rivulet_producer_stop rivulet_producer_registry_get_stop (RivuletProducerType type) {
  return rivulet_producer_registry_get (type)->stop;
}