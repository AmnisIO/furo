#include "RivuletSample.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletSample {
  RIVULET_OPERATOR_DEFINITION
  RivuletStream *_to_sample;
  RivuletListener *_listener;
  Boolean _has;
  int _value;
} RivuletSample;

static Boolean FALSE = 0;
static Boolean TRUE = 1;

typedef struct SampleListener {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
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
  rivulet_listener_registry_get_complete (internal_listener->listener_type) (internal_listener);
}

static Boolean _sample_listener_registered = 0;
static RivuletListenerType _sample_listener_listener_type = 0;
static RivuletProducerType _sample_listener_producer_type = 0;

static void _sample_listener_register () {
  if (_sample_listener_registered) return;
  _sample_listener_listener_type = rivulet_listener_registry_register (_sample_listener_next, _sample_listener_complete);
  _sample_listener_registered = 1;
}

SampleListener *sample_listener_create (RivuletSample *operator) {
  SampleListener *listener = xmalloc (sizeof (SampleListener));
  _sample_listener_register ();
  listener->listener_type = _sample_listener_listener_type;
  listener->producer_type = _sample_listener_producer_type;
  listener->operator = operator;
  return listener;
}

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletSample *operator = (RivuletSample *) self;
  operator->out = (RivuletStream *) out;
  operator->_has = FALSE;
  rivulet_stream_add_listener (operator->_to_sample, operator->_listener);
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletSample *operator = (RivuletSample *) self;
  rivulet_stream_remove_listener (operator->_to_sample, operator->_listener);
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletSample *operator = (RivuletSample *) self;
  if (!operator->_has) return;
  rivulet_operator_out_next (operator, operator->_value);
  operator->_has = FALSE;
}

static void _complete (RivuletListener *self) {
  RivuletSample *operator = (RivuletSample *) self;
  RivuletListener *out = (RivuletListener *) operator->out;
  if (out == NULL) return;
  if (operator->_has) rivulet_operator_out_next (operator, operator->_value);
  operator->_has = FALSE;
  rivulet_operator_out_complete (operator);
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

static RivuletProducer *rivulet_sample_create (RivuletStream *in, RivuletStream *to_sample) {
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

RivuletStream *rivulet_stream_sample (RivuletStream *in, RivuletStream *to_sample) {
  return rivulet_stream_create (rivulet_sample_create (in, to_sample));
}
