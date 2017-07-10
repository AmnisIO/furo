#include "RivuletFilter.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletFilter {
  RIVULET_OPERATOR_DEFINITION
  rivulet_filter_function filter;
} RivuletFilter;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletFilter *operator = (RivuletFilter *) self;
  RivuletStream *stream = (RivuletStream *) out;
  operator->out = stream;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletFilter *operator = (RivuletFilter *) self;
  Boolean passes = operator->filter (value);
  if (!passes) return;
  rivulet_operator_out_next (operator, value);
}

static void _complete (RivuletListener *self) {
  RivuletFilter *operator = (RivuletFilter *) self;
  rivulet_operator_out_complete (operator);
}

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_filter_create (RivuletStream *in, rivulet_filter_function filter) {
  RivuletFilter *operator = xmalloc (sizeof (RivuletFilter));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->filter = filter;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_filter (RivuletStream *in, rivulet_stream_filter_function filter) {
  return rivulet_stream_create (rivulet_filter_create (in, filter));
}
