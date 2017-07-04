#include "RivuletProducer.h"

static void _start (struct RivuletProducerInternal *self, struct RivuletListenerInternal *listener) {
  RivuletProducer *producer = (RivuletProducer *) self;
  producer->start (producer, listener);
}
static void _stop (struct RivuletProducerInternal *self) {
  RivuletProducer *producer = (RivuletProducer *) self;
  producer->stop (producer);
};

void rivulet_producer_initialize (RivuletProducer *producer, rivulet_producer_start start,
                                  rivulet_producer_stop stop) {
  producer->type = RIVULET_OBSERVABLE_TYPE_PRODUCER;
  producer->start = start;
  producer->stop = stop;
  producer->_start = _start;
  producer->_stop = _stop;
}

RivuletProducer *rivulet_producer_create (rivulet_producer_start start, rivulet_producer_stop stop) {
  RivuletProducer *producer = xmalloc (sizeof (RivuletProducer));
  rivulet_producer_initialize (producer, start, stop);
  return producer;
}