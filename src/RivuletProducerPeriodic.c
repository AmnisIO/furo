#include "RivuletProducerPeriodic.h"
#include "RivuletListernerManager.h"

static void _tick (void *self) {
  RivuletProducerPeriodic *producer = self;
  RivuletListener *listener = producer->_listener;
  rivulet_listener_internal_next next = rivulet_listener_internal_next_get (listener);
  next (listener, 0);
}

static void _periodic_start (RivuletProducer *self, RivuletListener *listener) {
  RivuletProducerPeriodic *producer = (RivuletProducerPeriodic *) self;
  producer->_listener = listener;
  producer->_task_id = rivulet_timer->set_interval (_tick, producer, producer->_period);
}

static void _periodic_stop (RivuletProducer *self) {
  RivuletProducerPeriodic *producer = (RivuletProducerPeriodic *) self;
  rivulet_timer->clear_task (producer->_task_id);
}

RivuletProducer *rivulet_producer_periodic_create (int period) {
  RivuletProducerPeriodic *producer = xmalloc (sizeof (RivuletProducerPeriodic));
  rivulet_producer_initialize ((RivuletProducer *) producer, _periodic_start, _periodic_stop);
  producer->_period = period;
  return (RivuletProducer *) producer;
}
