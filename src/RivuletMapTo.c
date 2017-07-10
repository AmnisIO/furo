#include "RivuletMapTo.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletMapTo {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int value;
} RivuletMapTo;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  operator->out = (RivuletStream *) out;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  rivulet_operator_out_next (operator, operator->value);
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

static RivuletProducer *rivulet_map_to_create (RivuletStream *in, int value) {
  RivuletMapTo *operator = xmalloc (sizeof (RivuletMapTo));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->value = value;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_map_to (RivuletStream *in, int value) {
  return rivulet_stream_create (rivulet_map_to_create (in, value));
}

