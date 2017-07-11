#include "RivuletPeriodic.h"
#include "RivuletTimer.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

typedef struct RivuletPeriodic {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  int _period;
  RivuletListener *_listener;
  RivuletTaskIdentifier _task_id;
} RivuletPeriodic;

static void _tick (void *self) {
  RivuletPeriodic *producer = self;
  RivuletListener *listener = producer->_listener;
  rivulet_listener_registry_get_next (listener->listener_type) (listener, 0);
}

static void _start (RivuletProducer *self, RivuletListener *listener) {
  RivuletPeriodic *producer = (RivuletPeriodic *) self;
  producer->_listener = listener;
  producer->_task_id = rivulet_timer->set_interval (_tick, producer, producer->_period);
}

static void _stop (RivuletProducer *self) {
  RivuletPeriodic *producer = (RivuletPeriodic *) self;
  rivulet_timer->clear_task (producer->_task_id);
}

static Boolean _registered = 0;
static RivuletListenerType _listener_type = 0;
static RivuletProducerType _producer_type = 0;

static void _register () {
  if (_registered) return;
  _producer_type = rivulet_producer_registry_register (_start, _stop);
  _registered = 1;
}

static RivuletProducer *rivulet_producer_periodic_create (int period) {
  RivuletPeriodic *producer = xmalloc (sizeof (RivuletPeriodic));
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  producer->_period = period;
  return (RivuletProducer *) producer;
}

RivuletStream *rivulet_stream_periodic (int period) {
  return rivulet_stream_create (rivulet_producer_periodic_create (period));
}
