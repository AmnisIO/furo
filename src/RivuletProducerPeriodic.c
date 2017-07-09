#include "RivuletProducerPeriodic.h"
#include "RivuletListenerRegistry.h"
#include "RivuletProducerRegistry.h"

static void _tick (void *self) {
  RivuletProducerPeriodic *producer = self;
  RivuletListener *listener = producer->_listener;
  rivulet_listener_registry_get_next (listener->listener_type) (listener, 0);
}

static void _start (RivuletProducer *self, RivuletListener *listener) {
  RivuletProducerPeriodic *producer = (RivuletProducerPeriodic *) self;
  producer->_listener = listener;
  producer->_task_id = rivulet_timer->set_interval (_tick, producer, producer->_period);
}

static void _stop (RivuletProducer *self) {
  RivuletProducerPeriodic *producer = (RivuletProducerPeriodic *) self;
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

RivuletProducer *rivulet_producer_periodic_create (int period) {
  RivuletProducerPeriodic *producer = xmalloc (sizeof (RivuletProducerPeriodic));
  _register ();
  producer->listener_type = _listener_type;
  producer->producer_type = _producer_type;
  producer->_period = period;
  return (RivuletProducer *) producer;
}
