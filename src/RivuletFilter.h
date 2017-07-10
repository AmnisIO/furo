#ifndef RIVULET_FILTER_H
#define RIVULET_FILTER_H

#include "RivuletStream.h"

typedef Boolean (*rivulet_filter_function) (int);
RivuletStream *rivulet_stream_filter (RivuletStream *, rivulet_stream_filter_function);

#endif //RIVULET_FILTER_H
