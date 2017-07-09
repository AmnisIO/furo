#ifndef RIVULET_STREAM_H
#define RIVULET_STREAM_H

#include "RivuletListener.h"
#include "RivuletProducer.h"
#include "RivuletArray.h"
#include "Boolean.h"
#include "RivuletProducerFromArray.h"

typedef int (*rivulet_stream_map_function) (int value);
typedef Boolean (*rivulet_stream_filter_function) (int value);

typedef struct RivuletStream {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletProducer *_producer;
  RivuletArray *_internal_listeners;
  int _stop_id;
  void (*_add) (struct RivuletStream *, RivuletListener *);
  void (*_remove) (struct RivuletStream *, RivuletListener *);
  void (*add_listener) (struct RivuletStream *, RivuletListener *);
  void (*remove_listener) (struct RivuletStream *, RivuletListener *);
  struct RivuletStream *(*map) (struct RivuletStream *, rivulet_stream_map_function);
  struct RivuletStream *(*map_to) (struct RivuletStream *, int);
  struct RivuletStream *(*filter) (struct RivuletStream *, rivulet_stream_filter_function);
  struct RivuletStream *(*take) (struct RivuletStream *, int);
  struct RivuletStream *(*drop) (struct RivuletStream *, int);
  struct RivuletStream *(*last) (struct RivuletStream *);
  struct RivuletStream *(*sample) (struct RivuletStream *, struct RivuletStream *);
  struct RivuletStream *(*delay) (struct RivuletStream *, int);
} RivuletStream;

RivuletStream *rivulet_stream_create (RivuletProducer *producer);
RivuletStream *rivulet_stream_never ();
RivuletStream *rivulet_stream_empty ();
RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *array);
RivuletStream *rivulet_stream_from_array (int *array, int size);
RivuletStream *rivulet_stream_periodic (int period);
RivuletStream *rivulet_stream_merge (int count, ...);

#endif // RIVULET_STREAM_H
