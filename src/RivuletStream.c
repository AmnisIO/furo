#include "RivuletStream.h"
#include "RivuletMap.h"
#include "RivuletProducerPeriodic.h"
#include "RivuletMapTo.h"
#include "RivuletFilter.h"
#include "RivuletTake.h"
#include "RivuletDrop.h"
#include "RivuletLast.h"
#include "RivuletDelay.h"
#include "RivuletSample.h"

int STOP_ID_NONE = 0;

int ERROR_NONE = 0;

static void _next (RivuletListenerInternal *self, int value) {
  RivuletStream *stream = (RivuletStream *) self;
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    RivuletListenerInternal *listener = rivulet_array_get (internal_listeners, i);
    rivulet_listener_internal_next_get (listener) (listener, value);
  }
}

static void _teardown (RivuletStream *self) {
  if (rivulet_array_length (self->_internal_listeners) == 0) return;
  if (self->_producer != NULL) self->_producer->_stop (self->_producer);
  rivulet_array_clear (self->_internal_listeners);
}

static void _complete (RivuletListenerInternal *self) {
  RivuletStream *stream = (RivuletStream *) self;
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    RivuletListenerInternal *listener = rivulet_array_get (internal_listeners, i);
    rivulet_listener_internal_complete_get (listener) (listener);
  }
  _teardown (stream);
}

static void _stop_now (RivuletStream *self) {
  if (self->_producer != NULL) self->_producer->_stop (self->_producer);
  self->_stop_id = STOP_ID_NONE;
}

static void _add (RivuletStream *stream, RivuletListenerInternal *listener) {
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_push (internal_listeners, listener);
  if (length > 1) return;
  if (stream->_stop_id != STOP_ID_NONE) {
    rivulet_timer->clear_task (stream->_stop_id);
    stream->_stop_id = STOP_ID_NONE;
  } else {
    RivuletProducerInternal *producer = stream->_producer;
    if (producer != NULL) producer->_start (producer, (RivuletListenerInternal *) stream);
  }
}

static void _stop_stream (void *any) {
  RivuletStream *stream = (RivuletStream *) any;
  _stop_now (stream);
}

static void _remove (RivuletStream *stream, RivuletListenerInternal *listener) {
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int index = rivulet_array_index_of (internal_listeners, listener);
  if (index > -1) {
    int length = rivulet_array_remove (internal_listeners, index);
    if (stream->_producer != NULL && length == 0) {
      stream->_stop_id = rivulet_timer->set_timeout (_stop_stream, stream, 0);
    }
  }
}

static void _add_listener (RivuletStream *stream, RivuletListener *listener) {
  stream->_add (stream, (RivuletListenerInternal *) listener);
}

static void _remove_listener (RivuletStream *stream, RivuletListener *listener) {
  stream->_remove (stream, (RivuletListenerInternal *) listener);
}

static RivuletStream *_map (RivuletStream *self, rivulet_map_function map);
static RivuletStream *_map_to (RivuletStream *self, int value);
static RivuletStream *_filter (RivuletStream *self, rivulet_stream_filter_function filter);
static RivuletStream *_take (RivuletStream *self, int count);
static RivuletStream *_drop (RivuletStream *self, int count);
static RivuletStream *_last (RivuletStream *self);
static RivuletStream *_delay (RivuletStream *self, int delay);
static RivuletStream *_sample (RivuletStream *self, RivuletStream *to_sample);

static RivuletStream *_create (RivuletProducerInternal *producer) {
  RivuletStream *stream = xmalloc (sizeof (RivuletStream));
  stream->type = RIVULET_OBSERVABLE_TYPE_STREAM;
  stream->_producer = producer;
  stream->_next = _next;
  stream->_complete = _complete;
  stream->_add = _add;
  stream->_remove = _remove;
  rivulet_array_initialize (&(stream->_internal_listeners));
  stream->_stop_id = STOP_ID_NONE;
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

static RivuletStream *_map (RivuletStream *self, rivulet_map_function map) {
  return _create ((RivuletProducerInternal *) rivulet_map_create (self, map));
}

static RivuletStream *_map_to (RivuletStream *self, int value) {
  return _create ((RivuletProducerInternal *) rivulet_map_to_create (self, value));
}

static RivuletStream *_filter (RivuletStream *self, rivulet_filter_function filter) {
  return _create ((RivuletProducerInternal *) rivulet_filter_create (self, filter));
}

static RivuletStream *_take (RivuletStream *self, int count) {
  return _create ((RivuletProducerInternal *) rivulet_take_create (self, count));
}

static RivuletStream *_drop (RivuletStream *self, int count) {
  return _create ((RivuletProducerInternal *) rivulet_drop_create (self, count));
}

static RivuletStream *_last (RivuletStream *self) {
  return _create ((RivuletProducerInternal *) rivulet_last_create (self));
}

static RivuletStream *_delay (RivuletStream *self, int delay) {
  return _create ((RivuletProducerInternal *) byte_delay_create (self, delay));
}

static RivuletStream *_sample (RivuletStream *self, RivuletStream *to_sample) {
  return _create ((RivuletProducerInternal *) rivulet_sample_create (self, to_sample));
}

RivuletStream *rivulet_stream_create (RivuletProducer *producer) {
  return _create ((RivuletProducerInternal *) producer);
}

static void _never_start (struct RivuletProducer *self, struct RivuletListenerInternal *listener) {

}

static void _never_stop (struct RivuletProducer *self) {

}

static void _complete_immediately (struct RivuletProducer *self, struct RivuletListenerInternal *listener) {
  listener->_complete (listener);
}

RivuletStream *rivulet_stream_never () {
  return rivulet_stream_create (rivulet_producer_create (_never_start, _never_stop));
}

RivuletStream *rivulet_stream_empty () {
  return rivulet_stream_create (rivulet_producer_create (_complete_immediately, _never_stop));
}

RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *array) {
  return rivulet_stream_create (rivulet_producer_from_variable_length_array (array));
}

RivuletStream *rivulet_stream_from_array (int *array, int size) {
  return rivulet_stream_create (rivulet_producer_from_array (array, size));
}

RivuletStream *rivulet_stream_periodic (int period) {
  return rivulet_stream_create (rivulet_producer_periodic_create (period));
}

