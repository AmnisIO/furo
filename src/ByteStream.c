#include "ByteStream.h"
#include "ByteByteMap.h"
#include "ByteProducerPeriodic.h"
#include "ByteMapTo.h"
#include "ByteFilter.h"
#include "ByteTake.h"
#include "ByteDrop.h"
#include "ByteLast.h"
#include "ByteDelay.h"
#include "ByteSample.h"

int STOP_ID_NONE = 0;

int ERROR_NONE = 0;

static void _next (ByteListenerInternal *self, Byte value) {
  ByteStream *stream = (ByteStream *) self;
  VariableLengthArray *internal_listeners = stream->_internal_listeners;
  int length = internal_listeners->length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    ByteListenerInternal *listener = internal_listeners->get (internal_listeners, i);
    byte_listener_internal_next_get (listener) (listener, value);
  }
}

static void _error (ByteListenerInternal *self, int error) {
  ByteStream *stream = (ByteStream *) self;
  if (stream->_error_code != ERROR_NONE) return;
  stream->_error_code = error;
  VariableLengthArray *internal_listeners = stream->_internal_listeners;
  int length = internal_listeners->length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    ByteListenerInternal *listener = internal_listeners->get (internal_listeners, i);
    byte_listener_internal_error_get (listener) (listener, error);
  }
  stream->_teardown (stream);
}

static void _complete (ByteListenerInternal *self) {
  ByteStream *stream = (ByteStream *) self;
  VariableLengthArray *internal_listeners = stream->_internal_listeners;
  int length = internal_listeners->length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    ByteListenerInternal *listener = internal_listeners->get (internal_listeners, i);
    byte_listener_internal_complete_get (listener) (listener);
  }
  stream->_teardown (stream);
}

static void _teardown (ByteStream *self) {
  if (self->_internal_listeners->length (self->_internal_listeners) == 0) return;
  if (self->_producer != NULL) self->_producer->_stop (self->_producer);
  self->_error_code = ERROR_NONE;
  self->_internal_listeners->clear (self->_internal_listeners);
}

static void _stop_now (ByteStream *self) {
  if (self->_producer != NULL) self->_producer->_stop (self->_producer);
  self->_error_code = ERROR_NONE;
  self->_stop_id = STOP_ID_NONE;
}

static void _add (ByteStream *stream, ByteListenerInternal *listener) {
  VariableLengthArray *internal_listeners = stream->_internal_listeners;
  int length = internal_listeners->push (internal_listeners, listener);
  if (length > 1) return;
  if (stream->_stop_id != STOP_ID_NONE) {
    rivulet_timer->clear_task (stream->_stop_id);
    stream->_stop_id = STOP_ID_NONE;
  } else {
    ByteProducerInternal *producer = stream->_producer;
    if (producer != NULL) producer->_start (producer, (ByteListenerInternal *) stream);
  }
}

static void _stop_stream (void *any) {
  ByteStream *stream = (ByteStream *) any;
  stream->_stop_now (stream);
}

static void _remove (ByteStream *stream, ByteListenerInternal *listener) {
  VariableLengthArray *internal_listeners = stream->_internal_listeners;
  int index = internal_listeners->index_of (internal_listeners, listener);
  if (index > -1) {
    int length = internal_listeners->remove (internal_listeners, index);
    if (stream->_producer != NULL && length == 0) {
      stream->_error_code = ERROR_NONE;
      stream->_stop_id = rivulet_timer->set_timeout (_stop_stream, stream, 0);
    }
  }
}

static void _add_listener (ByteStream *stream, ByteListener *listener) {
  stream->_add (stream, (ByteListenerInternal *) listener);
}

static void _remove_listener (ByteStream *stream, ByteListener *listener) {
  stream->_remove (stream, (ByteListenerInternal *) listener);
}

typedef struct ByteSubscriptionImplementation {
  void (*unsubscribe) (struct ByteSubscription *self);
  ByteStream *stream;
  ByteListener *listener;
} ByteSubscriptionImplementation;

static void _unsubscribe (ByteStream *stream, ByteListener *listener) {
  stream->remove_listener (stream, listener);
}
static ByteStream *_map (ByteStream *self, byte_byte_map_function map);
static ByteStream *_map_to (ByteStream *self, Byte value);
static ByteStream *_filter (ByteStream *self, byte_steam_filter_function filter);
static ByteStream *_take (ByteStream *self, int count);
static ByteStream *_drop (ByteStream *self, int count);
static ByteStream *_last (ByteStream *self);
static ByteStream *_delay (ByteStream *self, int delay);
static ByteStream *_sample (ByteStream *self, ByteStream *to_sample);

static ByteStream *_create (ByteProducerInternal *producer) {
  ByteStream *stream = xmalloc (sizeof (ByteStream));
  stream->type = OBSERVABLE_TYPE_STREAM;
  stream->_producer = producer;
  stream->_next = _next;
  stream->_error = _error;
  stream->_complete = _complete;
  stream->_teardown = _teardown;
  stream->_stop_now = _stop_now;
  stream->_add = _add;
  stream->_remove = _remove;
  variable_length_array_initialize (&(stream->_internal_listeners));
  stream->_stop_id = STOP_ID_NONE;
  stream->_error_code = ERROR_NONE;
  stream->add_listener = _add_listener;
  stream->remove_listener = _remove_listener;
  stream->map = _map;
  stream->map_to = _map_to;
  stream->filter = _filter;
  stream->take = _take;
  stream->drop = _drop;
  stream->last = _last;
  stream->delay = _delay;
  stream->sample = _sample;
  return stream;
}

static ByteStream *_map (ByteStream *self, byte_byte_map_function map) {
  return _create ((ByteProducerInternal *) byte_byte_map_create (self, map));
}

static ByteStream *_map_to (ByteStream *self, Byte value) {
  return _create ((ByteProducerInternal *) byte_map_to_create (self, value));
}

static ByteStream *_filter (ByteStream *self, byte_filter_function filter) {
  return _create ((ByteProducerInternal *) byte_filter_create (self, filter));
}

static ByteStream *_take (ByteStream *self, int count) {
  return _create ((ByteProducerInternal *) byte_take_create (self, count));
}

static ByteStream *_drop (ByteStream *self, int count) {
  return _create ((ByteProducerInternal *) byte_drop_create (self, count));
}

static ByteStream *_last (ByteStream *self) {
  return _create ((ByteProducerInternal *) byte_last_create (self));
}

static ByteStream *_delay (ByteStream *self, int delay) {
  return _create ((ByteProducerInternal *) byte_delay_create (self, delay));
}

static ByteStream *_sample (ByteStream *self, ByteStream *to_sample) {
  return _create ((ByteProducerInternal *) byte_sample_create (self, to_sample));
}

ByteStream *byte_stream_create (ByteProducer *producer) {
  return _create ((ByteProducerInternal *) producer);
}

static void _never_start (struct ByteProducer *self, struct ByteListenerInternal *listener) {

}

static void _never_stop (struct ByteProducer *self) {

}

static void _complete_immediately (struct ByteProducer *self, struct ByteListenerInternal *listener) {
  listener->_complete (listener);
}

ByteStream *byte_stream_never () {
  return byte_stream_create (byte_producer_create (_never_start, _never_stop));
}

ByteStream *byte_stream_empty () {
  return byte_stream_create (byte_producer_create (_complete_immediately, _never_stop));
}

ByteStream *byte_stream_from_variable_length_array (VariableLengthArray *array) {
  return byte_stream_create (byte_producer_from_variable_length_array (array));
}

ByteStream *byte_stream_from_array (Byte array[], int size) {
  return byte_stream_create (byte_producer_from_array (array, size));
}

ByteStream *byte_stream_periodic (int period) {
  return byte_stream_create (byte_producer_periodic_create (period));
}

