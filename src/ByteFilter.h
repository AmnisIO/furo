#ifndef GYRUS_ARDUINO_UNO_BYTEFILTER_H
#define GYRUS_ARDUINO_UNO_BYTEFILTER_H

#include "ByteByteOperator.h"

typedef Boolean (*byte_filter_function) (Byte value);

typedef struct ByteFilter {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
  byte_filter_function filter;
} ByteFilter;

ByteFilter *byte_filter_create (ByteStream *in, byte_filter_function filter);

#endif //GYRUS_ARDUINO_UNO_BYTEFILTER_H
