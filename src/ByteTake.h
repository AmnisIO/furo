#ifndef GYRUS_ARDUINO_UNO_BYTETAKE_H
#define GYRUS_ARDUINO_UNO_BYTETAKE_H


#include "ByteByteOperator.h"

typedef struct ByteTake {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  int _to_take;
  int _taken;
} ByteTake;

ByteTake *byte_take_create(ByteStream *in, int count);

#endif //GYRUS_ARDUINO_UNO_BYTETAKE_H
