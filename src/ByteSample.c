#include "ByteSample.h"

static Boolean FALSE = 0;
static Boolean TRUE = 1;

typedef struct SampleListener {
  ByteObservableType type;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteSample *operator;
} SampleListener;

static void _sample_listener_next (ByteListenerInternal *self, Byte value) {
  SampleListener *listener = (SampleListener *) self;
  if (listener->operator == NULL) return;
  listener->operator->_has = TRUE;
  listener->operator->_value = value;
}

static void _sample_listener_error (ByteListenerInternal *self, int error) {
  SampleListener *listener = (SampleListener *) self;
  ByteListenerInternal *internal_listener = (ByteListenerInternal *) listener->operator;
  if (internal_listener == NULL) return;
  byte_listener_internal_error_get (internal_listener) (internal_listener, error);
}

static void _sample_listener_complete (ByteListenerInternal *self) {
  SampleListener *listener = (SampleListener *) self;
  ByteListenerInternal *internal_listener = (ByteListenerInternal *) listener->operator;
  if (internal_listener == NULL) return;
  byte_listener_internal_complete_get (internal_listener) (internal_listener);
}

SampleListener *sample_listener_create (ByteSample *operator) {
  SampleListener *listener = xmalloc (sizeof (SampleListener));
  listener->type = OBSERVABLE_TYPE_LISTENER_INTERNAL;
  listener->_next = _sample_listener_next;
  listener->_error = _sample_listener_error;
  listener->_complete = _sample_listener_complete;
  listener->operator = operator;
  return listener;
}

static void _start (ByteProducerInternal *self, ByteListenerInternal *out) {
  ByteSample *operator = (ByteSample *) self;
  operator->out = (ByteStream *) out;
  operator->_has = FALSE;
  operator->_to_sample->_add (operator->_to_sample, operator->_listener);
  operator->in->_add (operator->in, (ByteListenerInternal *) operator);
}

static void _stop (ByteProducerInternal *self) {
  ByteSample *operator = (ByteSample *) self;
  operator->_to_sample->_remove (operator->_to_sample, operator->_listener);
  operator->in->_remove (operator->in, (ByteListenerInternal *) operator);
  operator->out = NULL;
}

static void _next (ByteListenerInternal *self, Byte value) {
  ByteSample *operator = (ByteSample *) self;
  if (!operator->_has) return;
  operator->out->_next ((ByteListenerInternal *) operator->out, operator->_value);
  operator->_has = FALSE;
}

static void _error (ByteListenerInternal *self, int error) {
  ByteSample *operator = (ByteSample *) self;
  if (operator->out == NULL) return;
  operator->out->_error ((ByteListenerInternal *) operator->out, error);
}

static void _complete (ByteListenerInternal *self) {
  ByteSample *operator = (ByteSample *) self;
  ByteListenerInternal *out = (ByteListenerInternal *) operator->out;
  if (out == NULL) return;
  if (operator->_has) byte_listener_internal_next_get (out) (out, operator->_value);
  operator->_has = FALSE;
  byte_listener_internal_complete_get (out) (out);
}

ByteSample *byte_sample_create (ByteStream *in, ByteStream *to_sample) {
  ByteSample *operator = xmalloc (sizeof (ByteSample));
  byte_byte_operator_initialize ((ByteByteOperator *) operator);
  operator->operation = OPERATOR_DROP;
  operator->in = in;
  operator->_start = _start;
  operator->_stop = _stop;
  operator->_next = _next;
  operator->_error = _error;
  operator->_complete = _complete;
  operator->_has = FALSE;
  operator->_to_sample = to_sample;
  operator->_listener = (ByteListenerInternal *) sample_listener_create (operator);
  return operator;
}
