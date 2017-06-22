#ifndef GYRUS_ARDUINO_UNO_BYTEDROP_H
#define GYRUS_ARDUINO_UNO_BYTEDROP_H

#include "ByteByteOperator.h"

typedef struct ByteDrop {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  int _to_drop;
  int _dropped;
} ByteDrop;

ByteDrop *byte_drop_create(ByteStream *in, int count);

#endif //GYRUS_ARDUINO_UNO_BYTEDROP_H
