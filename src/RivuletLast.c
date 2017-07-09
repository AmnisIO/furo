#include "RivuletLast.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletLast *operator = (RivuletLast *) self;
  operator->out = (RivuletStream *) out;
  operator->_has = FALSE;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletLast *operator = (RivuletLast *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletLast *operator = (RivuletLast *) self;
  operator->_has = TRUE;
  operator->_value = value;
}

static void _complete (RivuletListener *self) {
  RivuletLast *operator = (RivuletLast *) self;
  if (operator->out == NULL) return;
  if (operator->_has) {
    rivulet_operator_out_next (operator, operator->_value);
    rivulet_operator_out_complete (operator);;
  } else {
    rivulet_operator_out_complete (operator);;
  }
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

RivuletProducer *rivulet_last_create (RivuletStream *in) {
  RivuletLast *operator = xmalloc (sizeof (RivuletLast));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->_has = FALSE;
  return (RivuletProducer *) operator;
}

