#include "RivuletDrop.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletDrop *operator = (RivuletDrop *) self;
  operator->out = (RivuletStream *) out;
  operator->_dropped = 0;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletDrop *operator = (RivuletDrop *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletDrop *operator = (RivuletDrop *) self;
  if (operator->out == NULL) return;
  if (operator->_dropped++ < operator->_to_drop) return;
  rivulet_operator_out_next (operator, value);;
}

static void _complete (RivuletListener *self) {
  RivuletDrop *operator = (RivuletDrop *) self;
  rivulet_operator_out_complete (operator);;
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

RivuletProducer *rivulet_drop_create (RivuletStream *in, int count) {
  RivuletDrop *operator = xmalloc (sizeof (RivuletDrop));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->_to_drop = count;
  return (RivuletProducer *) operator;
}

