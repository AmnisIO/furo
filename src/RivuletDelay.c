#include "RivuletDelay.h"
#include "RivuletTimer.h"

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletDelay *operator = (RivuletDelay *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletDelay *operator = (RivuletDelay *) self;
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

typedef struct DelayedNext {
  RivuletDelay *operator;
  int value;
} DelayedNext;

static void *delayed_next_create (RivuletDelay *operator, int value) {
  DelayedNext *delayed_next = xmalloc (sizeof (DelayedNext));
  delayed_next->operator = operator;
  delayed_next->value = value;
  return delayed_next;
}

static void _send_next (void *self) {
  DelayedNext *delayed_emit = self;
  RivuletDelay *operator = delayed_emit->operator;
  if (operator->out == NULL) return;
  operator->out->_next ((RivuletListenerInternal *) operator->out, delayed_emit->value);
}

typedef struct DelayedComplete {
  RivuletDelay *operator;
} DelayedComplete;

static void *delayed_complete_create (RivuletDelay *operator) {
  DelayedComplete *delayed_complete = xmalloc (sizeof (DelayedComplete));
  delayed_complete->operator = operator;
  return delayed_complete;
}

static void _send_complete (void *self) {
  DelayedComplete *delayed_complete = self;
  RivuletDelay *operator = delayed_complete->operator;
  if (operator->out == NULL) return;
  operator->out->_complete ((RivuletListenerInternal *) operator->out);
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletDelay *operator = (RivuletDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_next, delayed_next_create (operator, value), operator->_delay);
}

static void _complete (RivuletListenerInternal *self) {
  RivuletDelay *operator = (RivuletDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_complete, delayed_complete_create (operator), operator->_delay);
}

RivuletDelay *byte_delay_create (RivuletStream *in, int delay) {
  RivuletDelay *operator = xmalloc (sizeof (RivuletDelay));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_DELAY;
  operator->in = in;
  operator->_delay = delay;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_complete = _complete;
  return operator;
}
