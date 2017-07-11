#include "RivuletFold.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "Rivulet.h"

typedef struct RivuletFold {
  RIVULET_OPERATOR_DEFINITION
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

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_fold_create (RivuletStream *in, rivulet_fold_function fold, int seed) {
  RivuletFold *operator = xmalloc (sizeof (RivuletFold));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->fold = fold;
  operator->seed = seed;
  operator->accumulated = seed;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_fold (RivuletStream *in, rivulet_fold_function fold, int seed) {
  return rivulet_stream_create (rivulet_fold_create (in, fold, seed));
}