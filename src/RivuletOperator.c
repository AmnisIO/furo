#include "RivuletOperator.h"
#include "RivuletListenerRegistry.h"

void rivulet_operator_in_add (void *operator) {
  RivuletOperator *self = operator;
  RivuletListener *listener = operator;
  self->in->rivulet_stream_add_listener (self->in, listener);
}

void rivulet_operator_in_remove (void *operator) {
  RivuletOperator *self = operator;
  RivuletListener *listener = operator;
  self->in->_rivulet_stream_remove_listener (self->in, listener);
}

void rivulet_operator_out_next (void *operator, int value) {
  RivuletOperator *self = operator;
  rivulet_listener_registry_get_next (self->out->listener_type) ((RivuletListener *) self->out, value);
}

void rivulet_operator_out_complete (void *operator) {
  RivuletOperator *self = operator;
  rivulet_listener_registry_get_complete (self->out->listener_type) ((RivuletListener *) self->out);
}