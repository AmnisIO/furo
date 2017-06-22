#ifndef GYRUS_ARDUINO_UNO_BYTEPRODUCERPERIODIC_H
#define GYRUS_ARDUINO_UNO_BYTEPRODUCERPERIODIC_H

#include "ByteProducer.h"
#include "RivuletTimer.h"

typedef struct ByteProducerPeriodic {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_producer_start start;
  byte_producer_stop stop;
  int _period;
  ByteListenerInternal *_listener;
  RivuletTaskIdentifier _task_id;
} ByteProducerPeriodic;

ByteProducer *byte_producer_periodic_create(int period);

#endif //GYRUS_ARDUINO_UNO_BYTEPRODUCERPERIODIC_H
