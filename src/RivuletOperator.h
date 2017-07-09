#ifndef RIVULET_OPERATOR_H
#define RIVULET_OPERATOR_H

#include "RivuletStream.h"

typedef struct RivuletOperator {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
} RivuletOperator;

#endif //RIVULET_OPERATOR_H
