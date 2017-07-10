#ifndef RIVULET_MAP_H
#define RIVULET_MAP_H

#include "RivuletStream.h"

typedef int (*rivulet_map_function) (int);
RivuletStream *rivulet_stream_map (RivuletStream *, rivulet_map_function);

#endif //RIVULET_MAP_H
