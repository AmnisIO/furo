#include "RivuletLast.h"
#include "RivuletErrors.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletLast *operator = (RivuletLast *) self;
  operator->out = (RivuletStream *) out;
  operator->_has = FALSE;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletLast *operator = (RivuletLast *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletLast *operator = (RivuletLast *) self;
  operator->_has = TRUE;
  operator->_value = value;
}

static void _error (RivuletListenerInternal *self, int error) {
  RivuletLast *operator = (RivuletLast *) self;
  if (operator->out == NULL) return;
  operator->out->_error ((RivuletListenerInternal *) operator->out, error);
}

static void _complete (RivuletListenerInternal *self) {
  RivuletLast *operator = (RivuletLast *) self;
  if (operator->out == NULL) return;
  if (operator->_has) {
    operator->out->_next ((RivuletListenerInternal *) operator->out, operator->_value);
    operator->out->_complete ((RivuletListenerInternal *) operator->out);
  } else {
    operator->out->_error ((RivuletListenerInternal *) operator->out,
                           RIVULET_ERROR_LAST_NO_VALUE_IN_LAST_INPUT);
  }
}

RivuletLast *rivulet_last_create (RivuletStream *in) {
  RivuletLast *operator = xmalloc (sizeof (RivuletLast));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_LAST;
  operator->in = in;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  operator->_has = FALSE;
  return operator;
}

