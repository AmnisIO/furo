#include "RivuletMap.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletMap *operator = (RivuletMap *) self;
  operator->out = (RivuletStream *) out;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletMap *operator = (RivuletMap *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletMap *operator = (RivuletMap *) self;
  rivulet_operator_out_next (operator, operator->map (value));
}

static void _complete (RivuletListener *self) {
  rivulet_operator_out_complete (self);
}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _listener_type = rivulet_listener_registry_register (_next, _complete);
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

RivuletProducer *rivulet_map_create (RivuletStream *in, rivulet_map_function map) {
  RivuletMap *operator = xmalloc (sizeof (RivuletMap));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->map = map;
  return (RivuletProducer *) operator;
}
