#include "RivuletFold.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "Rivulet.h"

typedef struct RivuletFold {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletStream *in;
  RivuletStream *out;
  rivulet_fold_function fold;
  int accumulated;
  int seed;
} RivuletFold;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletFold *operator = (RivuletFold *) self;
  operator->out = (RivuletStream *) out;
  operator->accumulated = operator->seed;
  rivulet_operator_out_next (operator, operator->accumulated);
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletFold *operator = (RivuletFold *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
  operator->accumulated = operator->seed;
}

static void _next (RivuletListener *self, int value) {
  RivuletFold *operator = (RivuletFold *) self;
  if (operator->out == NULL) return;
  operator->accumulated = operator->fold (operator->accumulated, value);
  rivulet_operator_out_next (operator, operator->accumulated);
}

static void _complete (RivuletListener *self) {
  RivuletFold *operator = (RivuletFold *) self;
  if (operator->out == NULL) return;
  rivulet_operator_out_complete (self);
}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _listener_type = rivulet_listener_registry_register (_next, _complete);
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

static RivuletProducer *rivulet_fold_create (RivuletStream *in, rivulet_fold_function fold, int seed) {
  RivuletFold *operator = xmalloc (sizeof (RivuletFold));
  _register ();
  operator->listener_type = _listener_type;
  operator->producer_type = _producer_type;
  operator->in = in;
  operator->fold = fold;
  operator->seed = seed;
  operator->accumulated = seed;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_fold (RivuletStream *in, rivulet_fold_function fold, int seed) {
  return rivulet_stream_create (rivulet_fold_create (in, fold, seed));
}