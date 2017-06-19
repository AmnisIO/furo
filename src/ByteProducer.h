#ifndef RIVULET_BYTEPRODUCER_H
#define RIVULET_BYTEPRODUCER_H

#include "ByteListener.h"
#include "ByteProducerInternal.h"

typedef struct ByteProducer {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  void (*start) (struct ByteProducer *self, struct ByteListenerInternal *listener);
  void (*stop) (struct ByteProducer *self);
} ByteProducer;

typedef void (*byte_producer_start) (struct ByteProducer *self, struct ByteListenerInternal *listener);
typedef void (*byte_producer_stop) (struct ByteProducer *self);

ByteProducer *byte_producer_create(byte_producer_start, byte_producer_stop);
void byte_producer_initialize(ByteProducer *producer, byte_producer_start, byte_producer_stop);

#endif // RIVULET_BYTEPRODUCER_H