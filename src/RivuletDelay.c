#include "RivuletDelay.h"
#include "RivuletTimer.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletDelay {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  int _delay;
} RivuletDelay;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletDelay *operator = (RivuletDelay *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletDelay *operator = (RivuletDelay *) self;
  rivulet_operator_in_remove (operator);
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
  rivulet_operator_out_next (operator, delayed_emit->value);
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
  rivulet_operator_out_complete (operator);
}

static void _next (RivuletListener *self, int value) {
  RivuletDelay *operator = (RivuletDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_next, delayed_next_create (operator, value), operator->_delay);
}

static void _complete (RivuletListener *self) {
  RivuletDelay *operator = (RivuletDelay *) self;
  if (operator->out == NULL) return;
  rivulet_timer->set_timeout (_send_complete, delayed_complete_create (operator), operator->_delay);
}


static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _listener_type = rivulet_listener_registry_register (_next, _complete);
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

static RivuletProducer *rivulet_delay_create (RivuletStream *in, int delay) {
  RivuletDelay *operator = xmalloc (sizeof (RivuletDelay));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->_delay = delay;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_delay (RivuletStream *in, int delay) {
  return rivulet_stream_create (rivulet_delay_create (in, delay));
}
