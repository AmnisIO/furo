#include "RivuletTake.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletTake *operator = (RivuletTake *) self;
  operator->out = (RivuletStream *) out;
  operator->_taken = 0;
  if (operator->_to_take <= 0) operator->out->_complete ((RivuletListenerInternal *) operator->out);
  else operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletTake *operator = (RivuletTake *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletTake *operator = (RivuletTake *) self;
  if (operator->out == NULL) return;
  int taken = ++operator->_taken;
  int to_take = operator->_to_take;
  if (taken <= to_take) operator->out->_next ((RivuletListenerInternal *) operator->out, value);
  if (taken == to_take) operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

static void _complete (RivuletListenerInternal *self) {
  RivuletTake *operator = (RivuletTake *) self;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

RivuletTake *rivulet_take_create (RivuletStream *in, int count) {
  RivuletTake *operator = xmalloc (sizeof (RivuletTake));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_TAKE;
  operator->in = in;
  operator->_to_take = count;
  operator->_taken = 0;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_complete = _complete;
  return operator;
}

