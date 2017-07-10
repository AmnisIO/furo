#include "RivuletMap.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"
#include "RivuletOperator.h"

typedef struct RivuletMap {
  RIVULET_OPERATOR_DEFINITION
  rivulet_map_function map;
} RivuletMap;

static void _start (RivuletProducer *self, RivuletListener *out) {
  RivuletMap *operator = (RivuletMap *) self;
  operator->out = (RivuletStream *) out;
  rivulet_operator_in_add (operator);
}

static void _stop (RivuletProducer *self) {
  RivuletMap *operator = (RivuletMap *) self;
  rivulet_operator_in_remove (operator);
  operator->out = NULL;
}

static void _next (RivuletListener *self, int value) {
  RivuletMap *operator = (RivuletMap *) self;
  rivulet_operator_out_next (operator, operator->map (value));
}

static void _complete (RivuletListener *self) {
  rivulet_operator_out_complete (self);
}

RIVULET_OPERATOR_REGISTER_DEFINITION

static RivuletProducer *rivulet_map_create (RivuletStream *in, rivulet_map_function map) {
  RivuletMap *operator = xmalloc (sizeof (RivuletMap));
  RIVULET_OPERATOR_REGISTRATION
  operator->in = in;
  operator->map = map;
  return (RivuletProducer *) operator;
}

RivuletStream *rivulet_stream_map (RivuletStream *in, rivulet_map_function map) {
  return rivulet_stream_create (rivulet_map_create (in, map));
}