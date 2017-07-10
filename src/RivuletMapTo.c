#include "RivuletMapTo.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletMapTo {
  RIVULET_OPERATOR_DEFINITION
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

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_map_to_create (RivuletStream *in, int value) {
  RivuletMapTo *operator = xmalloc (sizeof (RivuletMapTo));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->value = value;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_map_to (RivuletStream *in, int value) {
  return rivulet_stream_create (rivulet_map_to_create (in, value));
}

