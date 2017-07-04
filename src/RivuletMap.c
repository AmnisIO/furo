#include "RivuletMap.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletMap *operator = (RivuletMap *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletMap *operator = (RivuletMap *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletMap *operator = (RivuletMap *) self;
  int mapped = operator->map (value);
  operator->out->_next ((RivuletListenerInternal *) operator->out, mapped);
}

static void _error (RivuletListenerInternal *self, int error) {
  RivuletMap *operator = (RivuletMap *) self;
  operator->out->_error ((RivuletListenerInternal *) operator->out, error);

}

static void _complete (RivuletListenerInternal *self) {
  RivuletMap *operator = (RivuletMap *) self;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

RivuletMap *rivulet_map_create (RivuletStream *in, rivulet_map_function map) {
  RivuletMap *operator = xmalloc (sizeof (RivuletMap));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_MAP;
  operator->in = in;
  operator->map = map;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}
