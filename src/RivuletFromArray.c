#include "RivuletFromArray.h"
#include "RivuletProducerRegistry.h"
#include "RivuletListenerRegistry.h"

typedef struct RivuletFromArray {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletArray *array;
} RivuletFromArray;

static void _start (RivuletProducer *self, RivuletListener *listener) {
  RivuletFromArray *producer = (RivuletFromArray *) self;
  RivuletArray *array = producer->array;
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int *value = rivulet_array_get (array, i);
    rivulet_listener_registry_get_next (listener->listener_type) (listener, *value);
  }
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

static RivuletProducer *rivulet_producer_from_variable_length_array (RivuletArray *array) {
  RivuletFromArray *producer = xmalloc (sizeof (RivuletFromArray));
  producer->array = array;
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  return (RivuletProducer *) producer;
}

static RivuletProducer *rivulet_producer_from_array (int *array, int size) {
  RivuletFromArray *producer = xmalloc (sizeof (RivuletFromArray));
  rivulet_array_initialize (&(producer->array));
  RivuletArray *varray = producer->array;
  for (int i = 0; i < size; i++) {
    rivulet_array_push (varray, array + i);
  }
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  return (RivuletProducer *) producer;
}

RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *array) {
  return rivulet_stream_create (rivulet_producer_from_variable_length_array (array));
}
RivuletStream *rivulet_stream_from_array (int *array, int size) {
  return rivulet_stream_create (rivulet_producer_from_array (array, size));
}

