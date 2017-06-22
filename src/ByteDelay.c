#include "ByteDelay.h"
#include "RivuletTimer.h"

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteDelay *operator = (ByteDelay *) self;
  ByteStream *stream = (ByteStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteDelay *operator = (ByteDelay *) self;
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

typedef struct DelayedNext {
  ByteDelay *operator;
  Byte value;
} DelayedNext;

static void *delayed_next_create (ByteDelay *operator, Byte value) {
  DelayedNext *delayed_next = xmalloc (sizeof (DelayedNext));
  delayed_next->operator = operator;
  delayed_next->value = value;
  return delayed_next;
}

static void _send_next (void *self) {
  DelayedNext *delayed_emit = self;
  ByteDelay *operator = delayed_emit->operator;
  if (operator->out == NULL) return;
  operator->out->_next ((ByteListenerInternal *) operator->out, delayed_emit->value);
}

typedef struct DelayedError {
  ByteDelay *operator;
  int error;
} DelayedError;

static void *delayed_error_create (ByteDelay *operator, int error) {
  DelayedError *delayed_error = xmalloc (sizeof (DelayedError));
  delayed_error->operator = operator;
  delayed_error->error = error;
  return delayed_error;
}

static void _send_error (void *self) {
  DelayedError *delayed_error = self;
  ByteDelay *operator = delayed_error->operator;
  if (operator->out == NULL) return;
  operator->out->_error ((ByteListenerInternal *) operator->out, delayed_error->error);
}

typedef struct DelayedComplete {
  ByteDelay *operator;
} DelayedComplete;

static void *delayed_complete_create (ByteDelay *operator) {
  DelayedComplete *delayed_complete = xmalloc (sizeof (DelayedComplete));
  delayed_complete->operator = operator;
  return delayed_complete;
}

static void _send_complete (void *self) {
  DelayedComplete *delayed_complete = self;
  ByteDelay *operator = delayed_complete->operator;
  if (operator->out == NULL) return;
  operator->out->_complete ((ByteListenerInternal *) operator->out);
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteDelay *operator = (ByteDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_next, delayed_next_create (operator, value), operator->_delay);
}

static void _error (ByteListenerInternal *self, int error) {
  ByteDelay *operator = (ByteDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_error, delayed_error_create (operator, error), operator->_delay);
}

static void _complete (ByteListenerInternal *self) {
  ByteDelay *operator = (ByteDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_complete, delayed_complete_create (operator), operator->_delay);
}

ByteDelay *byte_delay_create (ByteStream *in, int delay) {
  ByteDelay *operator = xmalloc (sizeof (ByteDelay));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_DELAY;
  operator->in = in;
  operator->_delay = delay;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  return operator;
}
