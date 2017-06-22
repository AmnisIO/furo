#include <RivuletTask.h>
#include <RivuletTimer.h>
#include <RivuletErrors.h>
#include "ByteStream.h"

VariableLengthArray *array = NULL;
ByteListener *listener = NULL;
int error_code = -1;

static Milliseconds _milliseconds = 0;

Milliseconds get_milliseconds () {
  return _milliseconds++;
}

extern void reset_milliseconds () {
  _milliseconds = 0;
}

void _next (ByteListener *self, Byte value) {
  array->push (array, (void *) (Size) value);
}
void _error (ByteListener *self, int error) {
  error_code = error;
}
void _complete (ByteListener *self) {

}

void initialize_tests () {
  array = variable_length_array_create ();
  listener = byte_listener_create (_next, _error, _complete);
  rivulet_timer_initialize (get_milliseconds);
}

void reset () {
  array->clear (array);
  error_code = -1;
  reset_milliseconds ();
  rivulet_timer->_tasks->clear (rivulet_timer->_tasks);
}

Byte add_one (Byte value) {
  return value + (Byte) 1;
}

void test_from_varray () {
  reset ();
  printf ("TEST Stream<Byte>.fromVariableLengthArray()\n");
  VariableLengthArray *one_to_four = variable_length_array_create ();
  int a = 1, b = 2, c = 3, d = 4;
  one_to_four->push (one_to_four, &a);
  one_to_four->push (one_to_four, &b);
  one_to_four->push (one_to_four, &c);
  one_to_four->push (one_to_four, &d);
  ByteStream *stream = byte_stream_from_variable_length_array (one_to_four);
  stream->add_listener (stream, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == i + 1) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_from_array () {
  reset ();
  printf ("TEST Stream<Byte>.fromArray()\n");
  Byte five_to_eight[4] = {5, 6, 7, 8};
  ByteStream *stream = byte_stream_from_array (five_to_eight, 4);
  stream->add_listener (stream, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == i + 5) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_map () {
  reset ();
  printf ("TEST Stream<Byte>.map()\n");
  Byte five_to_eight[4] = {5, 6, 7, 8};
  ByteStream *stream = byte_stream_from_array (five_to_eight, 4);
  ByteStream *stream_map = stream->map (stream, add_one);
  stream_map->add_listener (stream_map, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == i + 6) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_map_to () {
  reset ();
  printf ("TEST Stream<Byte>.mapTo()\n");
  Byte five_to_eight[4] = {5, 6, 7, 8};
  ByteStream *stream = byte_stream_from_array (five_to_eight, 4);
  ByteStream *stream_map_to = stream->map_to (stream, 10);
  stream_map_to->add_listener (stream_map_to, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == 10) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

Boolean greater_than_6 (Byte value) {
  return value > 6;
}

void test_filter () {
  reset ();
  printf ("TEST Stream<Byte>.filter()\n");
  Byte five_to_eight[4] = {5, 6, 7, 8};
  ByteStream *stream = byte_stream_from_array (five_to_eight, 4);
  ByteStream *stream_filter = stream->filter (stream, greater_than_6);
  stream_filter->add_listener (stream_filter, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value > 6) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_take () {
  reset ();
  printf ("TEST Stream<Byte>.take()\n");
  Byte one_to_four[4] = {1, 2, 3, 4};
  ByteStream *stream = byte_stream_from_array (one_to_four, 4);
  ByteStream *stream_take = stream->take (stream, 3);
  stream_take->add_listener (stream_take, listener);
  int length = array->length (array);
  if (length != 3) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == i + 1) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_drop () {
  reset ();
  printf ("TEST Stream<Byte>.drop()\n");
  Byte one_to_four[4] = {1, 2, 3, 4};
  int to_drop = 3;
  ByteStream *stream = byte_stream_from_array (one_to_four, 4);
  ByteStream *stream_drop = stream->drop (stream, to_drop);
  stream_drop->add_listener (stream_drop, listener);
  int length = array->length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    Byte value = (Byte) array->get (array, i);
    if (value == i + 1 + to_drop) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_last () {
  reset ();
  printf ("TEST Stream<Byte>.last()\n");
  Byte one_to_four[4] = {1, 2, 3, 4};
  ByteStream *stream = byte_stream_from_array (one_to_four, 4);
  ByteStream *stream_last = stream->last (stream);
  stream_last->add_listener (stream_last, listener);
  int length = array->length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  Byte value = (Byte) array->get (array, 0);
  if (value != 4) {
    printf ("FAILED\n");
    return;
  }
  reset ();
  free (stream);
  stream = byte_stream_empty ();
  free (stream_last);
  stream_last = stream->last (stream);
  stream_last->add_listener (stream_last, listener);
  length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  if (error_code != RIVULET_ERROR_LAST_NO_VALUE_IN_LAST_INPUT) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_periodic () {
  reset ();
  printf ("TEST Stream<Byte>.periodic()\n");
  ByteStream *stream_periodic = byte_stream_periodic (2);
  stream_periodic->add_listener (stream_periodic, listener);
  int length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length == 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  rivulet_timer->tick ();
  length = array->length (array);
  if (length != 2) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_delay () {
  reset ();
  printf ("TEST Stream<Byte>.delay()\n");
  ByteStream *stream_periodic = byte_stream_periodic (1);
  ByteStream *stream_delay = stream_periodic->delay(stream_periodic, 1);
  stream_delay->add_listener (stream_delay, listener);
  int length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length == 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length != 2) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}


int main () {
  initialize_tests ();
  test_from_varray ();
  test_from_array ();
  test_map ();
  test_map_to ();
  test_filter ();
  test_take ();
  test_drop ();
  test_last ();
  test_periodic ();
  test_delay ();
  return 0;
}
