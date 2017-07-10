#include "RivuletTake.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletTake {
  RIVULET_OPERATOR_DEFINITION
  int _to_take;
  int _taken;
} RivuletTake;

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
  if (taken <= to_take) rivulet_operator_out_next (operator, value);
  if (taken == to_take) rivulet_operator_out_complete (operator);
}

static void _complete (RivuletListener *self) {
  RivuletTake *operator = (RivuletTake *) self;
  rivulet_operator_out_complete (operator);
}

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_take_create (RivuletStream *in, int count) {
  RivuletTake *operator = xmalloc (sizeof (RivuletTake));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->_to_take = count;
  operator->_taken = 0;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_take (RivuletStream *in, int count) {
  return rivulet_stream_create (rivulet_take_create (in, count));
}

