#include "ByteLast.h"
#include "RivuletErrors.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteLast *operator = (ByteLast *) self;
  operator->out = (ByteStream *) out;
  operator->_has = FALSE;
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteLast *operator = (ByteLast *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteLast *operator = (ByteLast *) self;
  operator->_has = TRUE;
  operator->_value = value;
}

static void _error (ByteListenerInternal *self, int error) {
  ByteLast *operator = (ByteLast *) self;
  if (operator->out == NULL) return;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);
}

static void _complete (ByteListenerInternal *self) {
  ByteLast *operator = (ByteLast *) self;
  if (operator->out == NULL) return;
  if (operator->_has) {
    operator->out->_next ((ByteListenerInternal *) operator->out, operator->_value);
    operator->out->_complete ((ByteListenerInternal *) operator->out);
  } else {
    operator->out->_error ((ByteListenerInternal *) operator->out, RIVULET_ERROR_LAST_NO_VALUE_IN_LAST_INPUT);
  }
}

ByteLast *byte_last_create (ByteStream *in) {
  ByteLast *operator = xmalloc (sizeof (ByteLast));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_LAST;
  operator->in = in;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  operator->_has = FALSE;
  return operator;
}

