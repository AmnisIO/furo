#include "RivuletFilter.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletFilter *operator = (RivuletFilter *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletFilter *operator = (RivuletFilter *) self;
  Boolean passes = operator->filter (value);
  if (!passes) return;
  operator->out->_next ((RivuletListenerInternal *) operator->out, value);
}

static void _error (RivuletListenerInternal *self, int error) {
  RivuletFilter *operator = (RivuletFilter *) self;
  operator->out->_error ((RivuletListenerInternal *) operator->out, error);

}

static void _complete (RivuletListenerInternal *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

RivuletFilter *rivulet_filter_create (RivuletStream *in, rivulet_filter_function filter) {
  RivuletFilter *operator = xmalloc (sizeof (RivuletFilter));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_FILTER;
  operator->in = in;
  operator->filter = filter;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}
