#include "ByteStream.h"

VariableLengthArray *array = NULL;
ByteListener *listener = NULL;

void _next (ByteListener *self, Byte value) {
  array->push (array, (void *) (Size) value);
}
void _error (ByteListener *self, int error) {

}
void _complete (ByteListener *self) {

}

void initialize_tests () {
  array = variable_length_array_create ();
  listener = byte_listener_create (_next, _error, _complete);
}

void reset () {
  array->clear (array);
}

Byte add_one (Byte value) {
  return value + (Byte) 1;
}

void test_from_varray () {
  reset ();
  printf ("TEST Stream<Byte>.fromVariableLengthArray() ");
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
  printf ("TEST Stream<Byte>.fromArray() ");
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
  printf ("TEST Stream<Byte>.map() ");
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
  printf ("TEST Stream<Byte>.mapTo() ");
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
  printf ("TEST Stream<Byte>.filter() ");
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
  printf ("TEST Stream<Byte>.take() ");
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
  printf ("TEST Stream<Byte>.drop() ");
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

int main () {
  initialize_tests ();
  test_from_varray ();
  test_from_array ();
  test_map ();
  test_map_to ();
  test_filter ();
  test_take ();
  test_drop ();
  ByteStream *stream_periodic = byte_stream_periodic (40);
  printf ("stream_periodic_created\n");
  stream_periodic->add_listener (stream_periodic, listener);
  return 0;
}
