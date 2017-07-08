#include "RivuletDrop.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletDrop *operator = (RivuletDrop *) self;
  operator->out = (RivuletStream *) out;
  operator->_dropped = 0;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletDrop *operator = (RivuletDrop *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletDrop *operator = (RivuletDrop *) self;
  if (operator->out == NULL) return;
  if (operator->_dropped++ < operator->_to_drop) return;
  operator->out->_next ((RivuletListenerInternal *) operator->out, value);
}

static void _complete (RivuletListenerInternal *self) {
  RivuletDrop *operator = (RivuletDrop *) self;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

RivuletDrop *rivulet_drop_create (RivuletStream *in, int count) {
  RivuletDrop *operator = xmalloc (sizeof (RivuletDrop));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_DROP;
  operator->in = in;
  operator->_to_drop = count;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_complete = _complete;
  return operator;
}

