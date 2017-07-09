#include "RivuletFilter.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletFilter *operator = (RivuletFilter *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletFilter *operator = (RivuletFilter *) self;
  Boolean passes = operator->filter (value);
  if (!passes) return;
  rivulet_operator_out_next (operator, value);
}

static void _complete (RivuletListener *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  rivulet_operator_out_complete (operator);
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
RivuletProducer *rivulet_filter_create (RivuletStream *in, rivulet_filter_function filter) {
  RivuletFilter *operator = xmalloc (sizeof (RivuletFilter));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->filter = filter;
  return (RivuletProducer *) operator;
}
