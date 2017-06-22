#ifndef GYRUS_ARDUINO_UNO_BYTEDELAY_H
#define GYRUS_ARDUINO_UNO_BYTEDELAY_H

#include "ByteByteOperator.h"

typedef struct ByteDelay {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  int _delay;
} ByteDelay;

ByteDelay *byte_delay_create (ByteStream *in, int delay);

#endif //GYRUS_ARDUINO_UNO_BYTEDELAY_H
