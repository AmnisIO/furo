#include "RivuletProducerFromArray.h"
#include "RivuletListernerManager.h"

static void _from_array_start (RivuletProducer *self, RivuletListenerInternal *listener) {
  RivuletProducerFromArray *producer = (RivuletProducerFromArray *) self;
  RivuletArray *array = producer->array;
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int *value = rivulet_array_get (array, i);
    rivulet_listener_internal_next_get (listener) (listener, *value);
  }
  rivulet_listener_internal_complete_get (listener) (listener);
}

static void _from_array_stop (RivuletProducer *self) {

}

RivuletProducer *rivulet_producer_from_variable_length_array (RivuletArray *array) {
  RivuletProducerFromArray *producer = xmalloc (sizeof (RivuletProducerFromArray));
  producer->array = array;
  rivulet_producer_initialize ((RivuletProducer *) producer, _from_array_start, _from_array_stop);
  return (RivuletProducer *) producer;
}

RivuletProducer *rivulet_producer_from_array (int *array, int size) {
  RivuletProducerFromArray *producer = xmalloc (sizeof (RivuletProducerFromArray));
  variable_length_array_initialize (&(producer->array));
  RivuletArray *varray = producer->array;
  for (int i = 0; i < size; i++) {
    rivulet_array_push (varray, array + i);
  }
  rivulet_producer_initialize ((RivuletProducer *) producer, _from_array_start, _from_array_stop);
  return (RivuletProducer *) producer;
}