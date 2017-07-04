#ifndef RIVULET_LISTENERINTERNAL_H
#define RIVULET_LISTENERINTERNAL_H

#include "RivuletUtils.h"
#include "RivuletObservable.h"

typedef struct RivuletListenerInternal {
  RivuletObservableType type;
  void (*_next) (struct RivuletListenerInternal *self, int v);
  void (*_error) (struct RivuletListenerInternal *self, int e);
  void (*_complete) (struct RivuletListenerInternal *self);
} RivuletListenerInternal;

typedef void (*rivulet_listener_internal_next) (struct RivuletListenerInternal *self, int v);
typedef void (*rivulet_listener_internal_error) (struct RivuletListenerInternal *self, int e);
typedef void (*rivulet_listener_internal_complete) (struct RivuletListenerInternal *self);

#endif //RIVULET_LISTENERINTERNAL_H
