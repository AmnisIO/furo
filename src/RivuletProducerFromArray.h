#ifndef RIVULET_PRODUCERFROMARRAY_H
#define RIVULET_PRODUCERFROMARRAY_H


#include "RivuletProducer.h"
#include "RivuletArray.h"

typedef struct RivuletProducerFromArray {
  RivuletObservableType type;
  rivulet_producer_internal_start _start;
  rivulet_producer_internal_stop _stop;
  rivulet_producer_start start;
  rivulet_producer_stop stop;
  RivuletArray *array;
} RivuletProducerFromArray;

RivuletProducer *rivulet_producer_from_variable_length_array (RivuletArray *array);
RivuletProducer *rivulet_producer_from_array (int *array, int size);
#endif //RIVULET_PRODUCERFROMARRAY_H
