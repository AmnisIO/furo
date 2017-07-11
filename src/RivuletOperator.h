#ifndef RIVULET_OPERATOR_H
#define RIVULET_OPERATOR_H

#include "RivuletStream.h"

#define RIVULET_OPERATOR_DEFINITION   \
  RivuletListenerType listener_type;  \
  RivuletProducerType producer_type;  \
  RivuletStream *in;                  \
  RivuletStream *out;

#define RIVULET_OPERATOR_REGISTER_DEFINITION                                \
  static Boolean _registered = 0;                                           \
  static RivuletListenerType _listener_type = 0;                            \
  static RivuletProducerType _producer_type = 0;                            \
  static void _register () {                                                \
    if (_registered) return;                                                \
    _listener_type = rivulet_listener_registry_register (_next, _complete); \
    _producer_type = rivulet_producer_registry_register (_start, _stop);    \
    _registered = 1;                                                        \
  }

#define RIVULET_OPERATOR_REGISTRATION       \
  _register ();                             \
  operator->listener_type = _listener_type; \
  operator->producer_type = _producer_type;

typedef struct RivuletOperator {
  RIVULET_OPERATOR_DEFINITION
} RivuletOperator;

void rivulet_operator_in_add (void *);
void rivulet_operator_in_remove (void *);
void rivulet_operator_out_next(void *, int value);
void rivulet_operator_out_complete(void *);

#endif //RIVULET_OPERATOR_H
