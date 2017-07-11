#include "RivuletNever.h"
#include "RivuletProducerRegistry.h"

static void _start (struct RivuletProducer *self, struct RivuletListener *listener) {

}

static void _stop (struct RivuletProducer *self) {

}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

static RivuletProducer *rivulet_producer_never_create () {
  RivuletProducer *producer = xmalloc (sizeof (RivuletProducer));
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  return producer;
}

RivuletStream *rivulet_stream_never () {
  return rivulet_stream_create (rivulet_producer_never_create ());
}