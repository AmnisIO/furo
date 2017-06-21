#include "ByteTake.h"

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteTake *operator = (ByteTake *) self;
  operator->out = (ByteStream *) out;
  operator->_taken = 0;
  if (operator->_to_take <= 0) operator->out->_complete ((ByteListenerInternal *) operator->out);
  else operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteTake *operator = (ByteTake *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteTake *operator = (ByteTake *) self;
  if (operator->out == NULL) return;
  int taken = ++operator->_taken;
  int to_take = operator->_to_take;
  if (taken <= to_take) operator->out->_next ((ByteListenerInternal *) operator->out, value);
  if (taken == to_take) operator->out->_complete ((ByteListenerInternal *) operator->out);
}

static void _error (ByteListenerInternal *self, int error) {
  ByteTake *operator = (ByteTake *) self;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);

}

static void _complete (ByteListenerInternal *self) {
  ByteTake *operator = (ByteTake *) self;
  operator->out->_complete ((ByteListenerInternal *) operator->out);
}

ByteTake *byte_take_create (ByteStream *in, int count) {
  ByteTake *operator = xmalloc (sizeof (ByteTake));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_TAKE;
  operator->in = in;
  operator->_to_take = count;
  operator->_taken = 0;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}

