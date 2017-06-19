#ifndef RIVULET_BYTEBYTEOPERATOR_H
#define RIVULET_BYTEBYTEOPERATOR_H

#include "ByteProducerInternal.h"
#include "ByteStream.h"

typedef enum OperatorType {
  OPERATOR_UNKNOWN,
  OPERATOR_MAP,
  OPERATOR_MAP_TO
} OperatorType;

typedef struct ByteByteOperator {
  ByteObservableType type;
  byte_producer_internal_start _start;
  byte_producer_internal_stop _stop;
  byte_listener_internal_next _next;
  byte_listener_internal_error _error;
  byte_listener_internal_complete _complete;
  ByteStream *in;
  ByteStream *out;
  OperatorType operation;
} ByteByteOperator;

ByteByteOperator *byte_byte_operator_create ();
void byte_byte_operator_initialize (ByteByteOperator *operator);

#endif //RIVULET_BYTEBYTEOPERATOR_H
