#include "RivuletSample.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

typedef struct SampleListener {
  RivuletObservableType type;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletSample *operator;
} SampleListener;

static void _sample_listener_next (RivuletListenerInternal *self, int value) {
  SampleListener *listener = (SampleListener *) self;
  if (listener->operator == NULL) return;
  listener->operator->_has = TRUE;
  listener->operator->_value = value;
}

static void _sample_listener_complete (RivuletListenerInternal *self) {
  SampleListener *listener = (SampleListener *) self;
  RivuletListenerInternal *internal_listener = (RivuletListenerInternal *) listener->operator;
  if (internal_listener == NULL) return;
  rivulet_listener_internal_complete_get (internal_listener) (internal_listener);
}

SampleListener *sample_listener_create (RivuletSample *operator) {
  SampleListener *listener = xmalloc (sizeof (SampleListener));
  listener->type = RIVULET_OBSERVABLE_TYPE_LISTENER_INTERNAL;
  listener->_next = _sample_listener_next;
  listener->_complete = _sample_listener_complete;
  listener->operator = operator;
  return listener;
}

static void _start (RivuletProducerInternal *self, RivuletListenerInternal *out) {
  RivuletSample *operator = (RivuletSample *) self;
  operator->out = (RivuletStream *) out;
  operator->_has = FALSE;
  operator->_to_sample->_add (operator->_to_sample, operator->_listener);
  operator->in->_add (operator->in, (RivuletListenerInternal *) operator);
}

static void _stop (RivuletProducerInternal *self) {
  RivuletSample *operator = (RivuletSample *) self;
  operator->_to_sample->_remove (operator->_to_sample, operator->_listener);
  operator->in->_remove (operator->in, (RivuletListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (RivuletListenerInternal *self, int value) {
  RivuletSample *operator = (RivuletSample *) self;
  if (!operator->_has) return;
  operator->out->_next ((RivuletListenerInternal *) operator->out, operator->_value);
  operator->_has = FALSE;
}

static void _complete (RivuletListenerInternal *self) {
  RivuletSample *operator = (RivuletSample *) self;
  RivuletListenerInternal *out = (RivuletListenerInternal *) operator->out;
  if (out == NULL) return;
  if (operator->_has) rivulet_listener_internal_next_get (out) (out, operator->_value);
  operator->_has = FALSE;
  rivulet_listener_internal_complete_get (out) (out);
}

RivuletSample *rivulet_sample_create (RivuletStream *in, RivuletStream *to_sample) {
  RivuletSample *operator = xmalloc (sizeof (RivuletSample));
  byte_byte_operator_initialize ((RivuletOperator *) operator);
  operator->operation = RIVULET_OPERATOR_DROP;
  operator->in = in;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_complete = _complete;
  operator->_has = FALSE;
  operator->_to_sample = to_sample;
  operator->_listener = (RivuletListenerInternal *) sample_listener_create (operator);
  return operator;
}
