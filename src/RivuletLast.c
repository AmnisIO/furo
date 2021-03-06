#include "RivuletLast.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletLast {
  RIVULET_OPERATOR_DEFINITION
  Boolean _has;
  int _value;
} RivuletLast;

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
    rivulet_operator_out_complete (operator);
  } else {
    rivulet_operator_out_complete (operator);
  }
}

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_last_create (RivuletStream *in) {
  RivuletLast *operator = xmalloc (sizeof (RivuletLast));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->_has = FALSE;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_last (RivuletStream *in) {
  return rivulet_stream_create (rivulet_last_create (in));
}

