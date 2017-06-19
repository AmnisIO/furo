#ifndef GYRUS_ARDUINO_UNO_BYTEMAPTO_H
#define GYRUS_ARDUINO_UNO_BYTEMAPTO_H

#include "ByteByteOperator.h"

typedef struct ByteMapTo {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  Byte value;
} ByteMapTo;

ByteMapTo *byte_map_to_create(ByteStream *in, Byte value);

#endif //GYRUS_ARDUINO_UNO_BYTEMAPTO_H
