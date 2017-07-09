#ifndef RIVULET_OPERATOR_H
#define RIVULET_OPERATOR_H

#include "RivuletStream.h"

typedef struct RivuletOperator {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
} RivuletOperator;

void rivulet_operator_in_add (void *);
void rivulet_operator_in_remove (void *);
void rivulet_operator_out_next(void *, int value);
void rivulet_operator_out_complete(void *);

#endif //RIVULET_OPERATOR_H
