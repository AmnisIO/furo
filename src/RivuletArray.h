#ifndef RIVULET_VARIABLELENGTHARRAY_H
#define RIVULET_VARIABLELENGTHARRAY_H

#include "RivuletUtils.h"
#include "Boolean.h"

typedef struct RivuletArray {
  void **memory;
  Size allocated;
  Size used;
  int index;
} RivuletArray;

void rivulet_array_initialize (RivuletArray **);
RivuletArray *rivulet_array_create ();
int rivulet_array_length (RivuletArray *);
int rivulet_array_push (RivuletArray *, void *);
void rivulet_array_clear (RivuletArray *);
void rivulet_array_free (RivuletArray *);
void *rivulet_array_get (RivuletArray *, int);
void rivulet_array_insert (RivuletArray *, int, void *);
int rivulet_array_remove (RivuletArray *, int);
int rivulet_array_index_of (RivuletArray *, void *);
typedef Boolean (*rivulet_array_is_equal) (void *, void *);
int rivulet_array_find_index (RivuletArray *, void *, rivulet_array_is_equal);


#endif // RIVULET_VARIABLELENGTHARRAY_H
