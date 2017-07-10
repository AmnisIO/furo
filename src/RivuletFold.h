#ifndef RIVULET_RIVULETFOLD_H
#define RIVULET_RIVULETFOLD_H

#include "RivuletStream.h"

typedef int (*rivulet_fold_function) (int, int);
RivuletStream *rivulet_stream_fold (RivuletStream *, rivulet_fold_function, int);

#endif //RIVULET_RIVULETFOLD_H
