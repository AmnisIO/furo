#ifndef RIVULET_OPERATOR_H
#define RIVULET_OPERATOR_H

#include "RivuletProducerInternal.h"
#include "RivuletStream.h"

typedef enum OperatorType {
  RIVULET_OPERATOR_UNKNOWN,
  RIVULET_OPERATOR_MAP,
  RIVULET_OPERATOR_MAP_TO,
  RIVULET_OPERATOR_FILTER,
  RIVULET_OPERATOR_TAKE,
  RIVULET_OPERATOR_DROP,
  RIVULET_OPERATOR_LAST,
  RIVULET_OPERATOR_DELAY
} OperatorType;

typedef struct RivuletOperator {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_listener_internal_next _next;
  rivulet_listener_internal_complete _complete;
  RivuletStream *in;
  RivuletStream *out;
  OperatorType operation;
} RivuletOperator;

RivuletOperator *byte_byte_operator_create ();
void byte_byte_operator_initialize (RivuletOperator *operator);

#endif //RIVULET_OPERATOR_H
