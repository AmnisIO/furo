#ifndef RIVULET_STREAM_H
#define RIVULET_STREAM_H

#include "RivuletListener.h"
#include "RivuletProducer.h"
#include "RivuletArray.h"
#include "Boolean.h"
#include "RivuletProducerFromArray.h"
#include "RivuletListernerManager.h"

typedef int (*rivulet_stream_map_function) (int value);
typedef Boolean (*rivulet_stream_filter_function) (int value);

typedef struct RivuletStream {
  RivuletObservableType type;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletProducerInternal *_producer;
  RivuletArray *_internal_listeners;
  int _stop_id;
  void (*_add) (struct RivuletStream *self, RivuletListenerInternal *listener);
  void (*_remove) (struct RivuletStream *self, RivuletListenerInternal *listener);
  void (*add_listener) (struct RivuletStream *self, RivuletListener *listener);
  void (*remove_listener) (struct RivuletStream *self, RivuletListener *listener);
  struct RivuletStream *(*map) (struct RivuletStream *self, rivulet_stream_map_function map);
  struct RivuletStream *(*map_to) (struct RivuletStream *self, int value);
  struct RivuletStream *(*filter) (struct RivuletStream *self, rivulet_stream_filter_function filter);
  struct RivuletStream *(*take) (struct RivuletStream *self, int count);
  struct RivuletStream *(*drop) (struct RivuletStream *self, int count);
  struct RivuletStream *(*last) (struct RivuletStream *self);
  struct RivuletStream *(*sample) (struct RivuletStream *self, struct RivuletStream *to_sample);
  struct RivuletStream *(*delay) (struct RivuletStream *self, int delay);
} RivuletStream;

RivuletStream *rivulet_stream_create (RivuletProducer *producer);
RivuletStream *rivulet_stream_never ();
RivuletStream *rivulet_stream_empty ();
RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *array);
RivuletStream *rivulet_stream_from_array (int *array, int size);
RivuletStream *rivulet_stream_periodic (int period);
RivuletStream *rivulet_stream_merge (int count, ...);

#endif // RIVULET_STREAM_H
