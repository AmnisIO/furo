#ifndef RIVULET_PRODUCERFROMARRAY_H
#define RIVULET_PRODUCERFROMARRAY_H

#include "RivuletArray.h"
#include "RivuletStream.h"

RivuletStream *rivulet_stream_from_variable_length_array (RivuletArray *);
RivuletStream *rivulet_stream_from_array (int *, int);
#endif //RIVULET_PRODUCERFROMARRAY_H
