#include "RivuletListernerManager.h"
#include "RivuletOperator.h"

rivulet_listener_internal_next rivulet_listener_internal_next_get (RivuletListenerInternal *listener) {
  switch (listener->type) {
    case RIVULET_OBSERVABLE_TYPE_LISTENER:
    case RIVULET_OBSERVABLE_TYPE_LISTENER_INTERNAL:
    case RIVULET_OBSERVABLE_TYPE_STREAM:
      return listener->_next;
    case RIVULET_OBSERVABLE_TYPE_OPERATOR:
      return ((RivuletOperator *) listener)->_next;
    default:
      return NULL;
  }
}

rivulet_listener_internal_error rivulet_listener_internal_error_get (RivuletListenerInternal *listener) {
  switch (listener->type) {
    case RIVULET_OBSERVABLE_TYPE_LISTENER:
    case RIVULET_OBSERVABLE_TYPE_LISTENER_INTERNAL:
    case RIVULET_OBSERVABLE_TYPE_STREAM:
      return listener->_error;
    case RIVULET_OBSERVABLE_TYPE_OPERATOR:
      return ((RivuletOperator *) listener)->_error;
    default:
      return NULL;
  }
}

rivulet_listener_internal_complete
rivulet_listener_internal_complete_get (RivuletListenerInternal *listener) {
  switch (listener->type) {
    case RIVULET_OBSERVABLE_TYPE_LISTENER:
    case RIVULET_OBSERVABLE_TYPE_LISTENER_INTERNAL:
    case RIVULET_OBSERVABLE_TYPE_STREAM:
      return listener->_complete;
    case RIVULET_OBSERVABLE_TYPE_OPERATOR:
      return ((RivuletOperator *) listener)->_complete;
    default:
      return NULL;
  }
}
