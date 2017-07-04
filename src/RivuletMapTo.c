#include "RivuletMapTo.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  operator->out->_next ((RivuletListenerInternal *) operator->out, operator->value);
}

static void _error (RivuletListenerInternal *self, int error) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  operator->out->_error ((RivuletListenerInternal *) operator->out, error);

}

static void _complete (RivuletListenerInternal *self) {
  RivuletMapTo *operator = (RivuletMapTo *) self;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

RivuletMapTo *rivulet_map_to_create (RivuletStream *in, int value) {
  RivuletMapTo *operator = xmalloc (sizeof (RivuletMapTo));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_MAP_TO;
  operator->in = in;
  operator->value = value;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}

