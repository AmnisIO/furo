#include "ByteProducerPeriodic.h"
#include "ByteListernerManager.h"

static void _tick (void *self) {
  ByteProducerPeriodic *producer = self;
  ByteListenerInternal *listener = producer->_listener;
  byte_listener_internal_next next = byte_listener_internal_next_get (listener);
  next (listener, (Byte) 0);
}

static void _periodic_start (ByteProducer *self, ByteListenerInternal *listener) {
  ByteProducerPeriodic *producer = (ByteProducerPeriodic *) self;
  producer->_listener = listener;
  producer->_task_id = byte_stream_timer_set_interval (_tick, producer, producer->_period);
}

static void _periodic_stop (ByteProducer *self) {
  ByteProducerPeriodic *producer = (ByteProducerPeriodic *) self;
  byte_stream_timer_clear_timeout (producer->_task_id);
}

ByteProducer *byte_producer_periodic_create(int period) {
  ByteProducerPeriodic *producer = xmalloc(sizeof (ByteProducerPeriodic));
  byte_producer_initialize ((ByteProducer *)producer, _periodic_start, _periodic_stop);
  producer->_period = period;
  return (ByteProducer *) producer;
}
