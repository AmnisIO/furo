#include "RivuletEmpty.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"

static void _start (RivuletProducer *self, RivuletListener *listener) {
  rivulet_listener_registry_get_complete (listener->listener_type) (listener);
}

static void _stop (RivuletProducer *self) {

}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

static RivuletProducer *rivulet_producer_empty_create () {
  RivuletProducer *producer = xmalloc (sizeof (RivuletProducer));
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  return producer;
}

RivuletStream *rivulet_stream_empty() {
  return rivulet_stream_create (rivulet_producer_empty_create ());
}