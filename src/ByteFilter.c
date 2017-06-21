#include "ByteFilter.h"

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteFilter *operator = (ByteFilter *) self;
  ByteStream *stream = (ByteStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteFilter *operator = (ByteFilter *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteFilter *operator = (ByteFilter *) self;
  Boolean passes = operator->filter (value);
  if (!passes) return;
  operator->out->_next ((ByteListenerInternal *) operator->out, value);
}

static void _error (ByteListenerInternal *self, int error) {
  ByteFilter *operator = (ByteFilter *) self;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);

}

static void _complete (ByteListenerInternal *self) {
  ByteFilter *operator = (ByteFilter *) self;
  operator->out->_complete ((ByteListenerInternal *) operator->out);
}

ByteFilter *byte_filter_create (ByteStream *in, byte_filter_function filter) {
  ByteFilter *operator = xmalloc (sizeof (ByteFilter));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_FILTER;
  operator->in = in;
  operator->filter = filter;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}
