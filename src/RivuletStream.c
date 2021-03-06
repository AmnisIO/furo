#include "RivuletStream.h"
#include "RivuletMap.h"
#include "RivuletPeriodic.h"
#include "RivuletMapTo.h"
#include "RivuletFilter.h"
#include "RivuletTake.h"
#include "RivuletDrop.h"
#include "RivuletLast.h"
#include "RivuletDelay.h"
#include "RivuletSample.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

int STOP_ID_NONE = 0;

static void _next (RivuletListener *self, int value) {
  RivuletStream *stream = (RivuletStream *) self;
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    RivuletListener *listener = rivulet_array_get (internal_listeners, i);
    rivulet_listener_registry_get_next (listener->listener_type) (listener, value);
  }
}

static void _teardown (RivuletStream *self) {
  if (rivulet_array_length (self->_internal_listeners) == 0) return;
  RivuletProducer *producer = self->_producer;
  if (producer != NULL) rivulet_producer_registry_get_stop (producer->producer_type) (self->_producer);
  rivulet_array_clear (self->_internal_listeners);
}

static void _complete (RivuletListener *self) {
  RivuletStream *stream = (RivuletStream *) self;
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_length (internal_listeners);
  if (length == 0) return;
  for (int i = 0; i < length; i++) {
    RivuletListener *listener = rivulet_array_get (internal_listeners, i);
    rivulet_listener_registry_get_complete (listener->listener_type) (listener);
  }
  _teardown (stream);
}

static void _stop_now (RivuletStream *self) {
  RivuletProducer *producer = self->_producer;
  if (producer != NULL) rivulet_producer_registry_get_stop (producer->producer_type) (self->_producer);
  self->_stop_id = STOP_ID_NONE;
}

void rivulet_stream_add_listener (RivuletStream *stream, RivuletListener *listener) {
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int length = rivulet_array_push (internal_listeners, listener);
  if (length > 1) return;
  if (stream->_stop_id != STOP_ID_NONE) {
    rivulet_timer->clear_task (stream->_stop_id);
    stream->_stop_id = STOP_ID_NONE;
  } else {
    RivuletProducer *producer = stream->_producer;
    if (producer != NULL)
      rivulet_producer_registry_get_start (producer->producer_type) (producer, (RivuletListener *) stream);
  }
}

static void _stop_stream (void *any) {
  RivuletStream *stream = (RivuletStream *) any;
  _stop_now (stream);
}

void rivulet_stream_remove_listener (RivuletStream *stream, RivuletListener *listener) {
  RivuletArray *internal_listeners = stream->_internal_listeners;
  int index = rivulet_array_index_of (internal_listeners, listener);
  if (index > -1) {
    int length = rivulet_array_remove (internal_listeners, index);
    if (stream->_producer != NULL && length == 0) {
      stream->_stop_id = rivulet_timer->set_timeout (_stop_stream, stream, 0);
    }
  }
}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _listener_type = rivulet_listener_registry_register (_next, _complete);
  _registered = 1;
}

static RivuletStream *_create (RivuletProducer *producer) {
  RivuletStream *stream = xmalloc (sizeof (RivuletStream));
  _register ();
  stream->listener_type = _listener_type;
  stream->producer_type = _producer_type;
  stream->_producer = producer;
  rivulet_array_initialize (&(stream->_internal_listeners));
  stream->_stop_id = STOP_ID_NONE;
  return stream;
}

RivuletStream *rivulet_stream_create (RivuletProducer *producer) {
  return _create (producer);
}

