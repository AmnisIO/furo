#ifndef RIVULET_SAMPLE_H
#define RIVULET_SAMPLE_H

#include "RivuletOperator.h"

typedef struct RivuletSample {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
  RivuletStream *_to_sample;
  RivuletListenerInternal *_listener;
  Boolean _has;
  int _value;
} RivuletSample;

RivuletSample *rivulet_sample_create (RivuletStream *in, RivuletStream *to_sample);

#endif //RIVULET_SAMPLE_H
