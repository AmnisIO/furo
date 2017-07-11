#include "RivuletDrop.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletDrop {
  RIVULET_OPERATOR_DEFINITION
  int _to_drop;
  int _dropped;
} RivuletDrop;

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
  rivulet_operator_out_next (operator, value);
}

static void _complete (RivuletListener *self) {
  RivuletDrop *operator = (RivuletDrop *) self;
  rivulet_operator_out_complete (operator);
}

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_drop_create (RivuletStream *in, int count) {
  RivuletDrop *operator = xmalloc (sizeof (RivuletDrop));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->_to_drop = count;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_drop (RivuletStream *in, int count) {
  return rivulet_stream_create (rivulet_drop_create (in, count));
}

