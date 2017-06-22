#include "ByteDrop.h"

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteDrop *operator = (ByteDrop *) self;
  operator->out = (ByteStream *) out;
  operator->_dropped = 0;
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteDrop *operator = (ByteDrop *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteDrop *operator = (ByteDrop *) self;
  if (operator->out == NULL) return;
  if (operator->_dropped++ < operator->_to_drop) return;
  operator->out->_next ((ByteListenerInternal *) operator->out, value);
}

static void _error (ByteListenerInternal *self, int error) {
  ByteDrop *operator = (ByteDrop *) self;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);
}

static void _complete (ByteListenerInternal *self) {
  ByteDrop *operator = (ByteDrop *) self;
  operator->out->_complete ((ByteListenerInternal *) operator->out);
}

ByteDrop *byte_drop_create (ByteStream *in, int count) {
  ByteDrop *operator = xmalloc (sizeof (ByteDrop));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_DROP;
  operator->in = in;
  operator->_to_drop = count;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}

