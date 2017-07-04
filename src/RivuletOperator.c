#include "RivuletOperator.h"

RivuletOperator *byte_byte_operator_create () {
  RivuletOperator *operator = xmalloc (sizeof (RivuletOperator));
  byte_byte_operator_initialize (operator);
  return operator;
}

void byte_byte_operator_initialize (RivuletOperator *operator) {
  operator->type = RIVULET_OBSERVABLE_TYPE_OPERATOR;
}