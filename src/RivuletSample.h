#ifndef RIVULET_SAMPLE_H
#define RIVULET_SAMPLE_H

#include "RivuletOperator.h"

typedef struct RivuletSample {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  RivuletStream *_to_sample;
  RivuletListener *_listener;
  Boolean _has;
  int _value;
} RivuletSample;

RivuletProducer *rivulet_sample_create (RivuletStream *in, RivuletStream *to_sample);

#endif //RIVULET_SAMPLE_H
