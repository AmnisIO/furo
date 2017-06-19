#include "ByteMapTo.h"

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteMapTo *operator = (ByteMapTo *) self;
  ByteStream *stream = (ByteStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteMapTo *operator = (ByteMapTo *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteMapTo *operator = (ByteMapTo *) self;
  operator->out->_next ((ByteListenerInternal *) operator->out, operator->value);
}

static void _error (ByteListenerInternal *self, int error) {
  ByteMapTo *operator = (ByteMapTo *) self;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);

}

static void _complete (ByteListenerInternal *self) {
  ByteMapTo *operator = (ByteMapTo *) self;
  operator->out->_complete ((ByteListenerInternal *) operator->out);
}

ByteMapTo *byte_map_to_create (ByteStream *in, Byte value) {
  ByteMapTo *operator = xmalloc (sizeof (ByteMapTo));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_MAP_TO;
  operator->in = in;
  operator->value = value;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}

