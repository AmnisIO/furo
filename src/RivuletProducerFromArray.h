#ifndef RIVULET_PRODUCERFROMARRAY_H
#define RIVULET_PRODUCERFROMARRAY_H


#include "RivuletProducer.h"
#include "RivuletArray.h"

typedef struct RivuletProducerFromArray {
  RivuletListenerType listener_type;
  RivuletProducerType producer_type;
  RivuletArray *array;
} RivuletProducerFromArray;

RivuletProducer *rivulet_producer_from_variable_length_array (RivuletArray *array);
RivuletProducer *rivulet_producer_from_array (int *array, int size);
#endif //RIVULET_PRODUCERFROMARRAY_H
