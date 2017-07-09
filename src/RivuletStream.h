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
} RivuletStream;

RivuletStream *rivulet_stream_create (RivuletProducer *producer);
RivuletStream *rivulet_stream_never ();
RivuletStream *rivulet_stream_empty ();
RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *array);
RivuletStream *rivulet_stream_from_array (int *array, int size);
RivuletStream *rivulet_stream_periodic (int period);
RivuletStream *rivulet_stream_merge (int count, ...);
void rivulet_stream_add_listener (RivuletStream *, RivuletListener *);
void rivulet_stream_remove_listener (RivuletStream *, RivuletListener *);
RivuletStream *rivulet_stream_map (RivuletStream *, rivulet_stream_map_function);
RivuletStream *rivulet_stream_map_to (RivuletStream *, int);
RivuletStream *rivulet_stream_filter (RivuletStream *, rivulet_stream_filter_function);
RivuletStream *rivulet_stream_take (RivuletStream *, int);
RivuletStream *rivulet_stream_drop (RivuletStream *, int);
RivuletStream *rivulet_stream_last (RivuletStream *);
RivuletStream *rivulet_stream_sample (RivuletStream *, RivuletStream *);
RivuletStream *rivulet_stream_delay (RivuletStream *, int);

#endif // RIVULET_STREAM_H