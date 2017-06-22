#ifndef GYRUS_ARDUINO_UNO_BYTELAST_H
#define GYRUS_ARDUINO_UNO_BYTELAST_H

#include "ByteByteOperator.h"

typedef struct ByteLast {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  Boolean _has;
  Byte _value;
} ByteLast;

ByteLast *byte_last_create(ByteStream *in);

#endif //GYRUS_ARDUINO_UNO_BYTELAST_H
