#ifndef RIVULET_OPERATOR_H
#define RIVULET_OPERATOR_H

#include "RivuletStream.h"

typedef struct RivuletOperator {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
} RivuletOperator;

RivuletOperator *byte_byte_operator_create ();
void byte_byte_operator_initialize (RivuletOperator *operator);

#endif //RIVULET_OPERATOR_H
