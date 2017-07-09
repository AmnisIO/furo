#include "RivuletArray.h"

int rivulet_array_length (RivuletArray *array) {
  return array->index + 1;
}

int rivulet_array_push (RivuletArray *array, void *data) {
  size_t toallocate;
  size_t size = sizeof (void *);
  if ((array->allocated - array->used) < size) {
    toallocate = array->allocated == 0 ? size : (array->allocated * 2);
    array->memory = realloc (array->memory, toallocate);
    array->allocated = toallocate;
  }
  array->memory[++array->index] = data;
  array->used = array->used + size;
  return rivulet_array_length (array);
}

void rivulet_array_clear (RivuletArray *array) {
  int i;
  for (i = 0; i < rivulet_array_length (array); i++) {
    array->memory[i] = NULL;
  }
  array->used = 0;
  array->index = -1;
}

void rivulet_array_free (RivuletArray *array) {
  free (array->memory);
  free (array);
}

void *rivulet_array_get (RivuletArray *array, int index) {
  if (index < 0 || index > array->index) return NULL;
  return array->memory[index];
}

void rivulet_array_insert (RivuletArray *array, int index, void *data) {
  if (index < 0 || index > array->index) return;
  array->memory[index] = data;
}

int rivulet_array_remove (RivuletArray *array, int index) {
  int length = rivulet_array_length (array);
  if (length <= index) return length;
  int last_index = length - 1;
  if (last_index != index) {
    for (int i = index; i < length; i++) {
      rivulet_array_insert (array, i, rivulet_array_get (array, i + 1));
    }
  }
  Size size = sizeof (void *);
  array->memory[last_index] = NULL;
  array->used -= size;
  array->index--;
  return last_index;
}

int rivulet_array_index_of (RivuletArray *array, void *data) {
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    if (array->memory[i] == data) return i;
  }
  return -1;
};

int rivulet_array_find_index (RivuletArray *array, void *data, rivulet_array_is_equal equal) {
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    if (equal (array->memory[i], data) == 1) return i;
  }
  return -1;
}

void rivulet_array_initialize (RivuletArray **array) {
  *array = (RivuletArray *) malloc (sizeof (RivuletArray));
  (*array)->memory = NULL;
  (*array)->allocated = 0;
  (*array)->used = 0;
  (*array)->index = -1;
}

RivuletArray *rivulet_array_create () {
  RivuletArray *array = xmalloc (sizeof (RivuletArray));
  array->memory = NULL;
  array->allocated = 0;
  array->used = 0;
  array->index = -1;
  return array;
}


