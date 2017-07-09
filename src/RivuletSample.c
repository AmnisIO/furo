#include "RivuletSample.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

typedef struct SampleListener {
  RivuletObservableType type;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletSample *operator;
} SampleListener;

static void _sample_listener_next (RivuletListener *self, int value) {
  SampleListener *listener = (SampleListener *) self;
  if (listener->operator == NULL) return;
  listener->operator->_has = TRUE;
  listener->operator->_value = value;
}

static void _sample_listener_complete (RivuletListener *self) {
  SampleListener *listener = (SampleListener *) self;
  RivuletListener *internal_listener = (RivuletListener *) listener->operator;
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

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletSample *operator = (RivuletSample *) self;
  operator->out = (RivuletStream *) out;
  operator->_has = FALSE;
  operator->_to_sample->_add (operator->_to_sample, operator->_listener);
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletSample *operator = (RivuletSample *) self;
  operator->_to_sample->_remove (operator->_to_sample, operator->_listener);
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletSample *operator = (RivuletSample *) self;
  if (!operator->_has) return;
  operator->out->_next ((RivuletListener *) operator->out, operator->_value);
  operator->_has = FALSE;
}

static void _complete (RivuletListener *self) {
  RivuletSample *operator = (RivuletSample *) self;
  RivuletListener *out = (RivuletListener *) operator->out;
  if (out == NULL) return;
  if (operator->_has) rivulet_listener_internal_next_get (out) (out, operator->_value);
  operator->_has = FALSE;
  rivulet_listener_internal_complete_get (out) (out);
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

RivuletProducer *rivulet_sample_create (RivuletStream *in, RivuletStream *to_sample) {
  RivuletSample *operator = xmalloc (sizeof (RivuletSample));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->_has = FALSE;
  operator->_to_sample = to_sample;
  operator->_listener = (RivuletListener *) sample_listener_create (operator);
  return (RivuletProducer *) operator;
}
