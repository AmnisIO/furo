#ifndef RIVULET_STREAM_H
#define RIVULET_STREAM_H

#include "RivuletListener.h"
#include "RivuletProducer.h"
#include "RivuletArray.h"
#include "Boolean.h"
#include "RivuletTimer.h"

typedef Boolean (*rivulet_stream_filter_function) (int value);

typedef struct RivuletStream {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletProducer *_producer;
  RivuletArray *_internal_listeners;
  RivuletTaskIdentifier _stop_id;
} RivuletStream;

RivuletStream *rivulet_stream_create (RivuletProducer *producer);
void rivulet_stream_add_listener (RivuletStream *, RivuletListener *);
void rivulet_stream_remove_listener (RivuletStream *, RivuletListener *);
RivuletStream *rivulet_stream_drop (RivuletStream *, int);
RivuletStream *rivulet_stream_last (RivuletStream *);
RivuletStream *rivulet_stream_sample (RivuletStream *, RivuletStream *);
RivuletStream *rivulet_stream_delay (RivuletStream *, int);

#endif // RIVULET_STREAM_H
