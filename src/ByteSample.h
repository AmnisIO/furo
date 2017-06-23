#ifndef RIVULET_BYTESAMPLE_H
#define RIVULET_BYTESAMPLE_H

#include "ByteByteOperator.h"

typedef struct ByteSample {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  ByteStream *_to_sample;
  ByteListenerInternal *_listener;
  Boolean _has;
  Byte _value;
} ByteSample;

ByteSample *byte_sample_create(ByteStream *in, ByteStream *to_sample);

#endif //RIVULET_BYTESAMPLE_H
