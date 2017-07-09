#include "RivuletTake.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletTake *operator = (RivuletTake *) self;
  operator->out = (RivuletStream *) out;
  operator->_taken = 0;
  if (operator->_to_take <= 0) rivulet_operator_out_complete (operator);
  else rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletTake *operator = (RivuletTake *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletTake *operator = (RivuletTake *) self;
  if (operator->out == NULL) return;
  int taken = ++operator->_taken;
  int to_take = operator->_to_take;
  if (taken <= to_take) rivulet_operator_out_next (operator, value);;
  if (taken == to_take) rivulet_operator_out_complete (operator);;
}

static void _complete (RivuletListener *self) {
  RivuletTake *operator = (RivuletTake *) self;
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

RivuletProducer *rivulet_take_create (RivuletStream *in, int count) {
  RivuletTake *operator = xmalloc (sizeof (RivuletTake));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->_to_take = count;
  operator->_taken = 0;
  return (RivuletProducer *) operator;
}

