#include <RivuletTask.h>
#include <RivuletTimer.h>
#include "RivuletStream.h"

VariableLengthArray *array = NULL;
RivuletListener *listener = NULL;

static Milliseconds _milliseconds = 0;

Milliseconds get_milliseconds () {
  return _milliseconds++;
}

extern void reset_milliseconds () {
  _milliseconds = 0;
}

void _next (RivuletListener *self, int value) {
  array->push (array, (void *) (Size) value);
}
void _complete (RivuletListener *self) {

}

void initialize_tests () {
  array = variable_length_array_create ();
  listener = rivulet_listener_create (_next, _complete);
  rivulet_timer_initialize (get_milliseconds);
}

void reset () {
  array->clear (array);
  reset_milliseconds ();
  rivulet_timer->_tasks->clear (rivulet_timer->_tasks);
}

int add_one (int value) {
  return value + 1;
}

void test_from_varray () {
  reset ();
  printf ("TEST RivuletStream.fromVariableLengthArray()\n");
  VariableLengthArray *one_to_four = variable_length_array_create ();
  int a = 1, b = 2, c = 3, d = 4;
  one_to_four->push (one_to_four, &a);
  one_to_four->push (one_to_four, &b);
  one_to_four->push (one_to_four, &c);
  one_to_four->push (one_to_four, &d);
  RivuletStream *stream = rivulet_stream_from_variable_length_array (one_to_four);
  stream->add_listener (stream, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == i + 1) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_from_array () {
  reset ();
  printf ("TEST RivuletStream.fromArray()\n");
  int five_to_eight[4] = {5, 6, 7, 8};
  RivuletStream *stream = rivulet_stream_from_array (five_to_eight, 4);
  stream->add_listener (stream, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == i + 5) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_map () {
  reset ();
  printf ("TEST RivuletStream.map()\n");
  int five_to_eight[4] = {5, 6, 7, 8};
  RivuletStream *stream = rivulet_stream_from_array (five_to_eight, 4);
  RivuletStream *stream_map = stream->map (stream, add_one);
  stream_map->add_listener (stream_map, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == i + 6) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_map_to () {
  reset ();
  printf ("TEST RivuletStream.mapTo()\n");
  int five_to_eight[4] = {5, 6, 7, 8};
  RivuletStream *stream = rivulet_stream_from_array (five_to_eight, 4);
  RivuletStream *stream_map_to = stream->map_to (stream, 10);
  stream_map_to->add_listener (stream_map_to, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == 10) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

Boolean greater_than_6 (int value) {
  return value > 6;
}

void test_filter () {
  reset ();
  printf ("TEST RivuletStream.filter()\n");
  int five_to_eight[4] = {5, 6, 7, 8};
  RivuletStream *stream = rivulet_stream_from_array (five_to_eight, 4);
  RivuletStream *stream_filter = stream->filter (stream, greater_than_6);
  stream_filter->add_listener (stream_filter, listener);
  int length = array->length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value > 6) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_take () {
  reset ();
  printf ("TEST RivuletStream.take()\n");
  int one_to_four[4] = {1, 2, 3, 4};
  RivuletStream *stream = rivulet_stream_from_array (one_to_four, 4);
  RivuletStream *stream_take = stream->take (stream, 3);
  stream_take->add_listener (stream_take, listener);
  int length = array->length (array);
  if (length != 3) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == i + 1) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_drop () {
  reset ();
  printf ("TEST RivuletStream.drop()\n");
  int one_to_four[4] = {1, 2, 3, 4};
  int to_drop = 3;
  RivuletStream *stream = rivulet_stream_from_array (one_to_four, 4);
  RivuletStream *stream_drop = stream->drop (stream, to_drop);
  stream_drop->add_listener (stream_drop, listener);
  int length = array->length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    int value = (int) array->get (array, i);
    if (value == i + 1 + to_drop) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_last () {
  reset ();
  printf ("TEST RivuletStream.last()\n");
  int one_to_four[4] = {1, 2, 3, 4};
  RivuletStream *stream = rivulet_stream_from_array (one_to_four, 4);
  RivuletStream *stream_last = stream->last (stream);
  stream_last->add_listener (stream_last, listener);
  int length = array->length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  int value = (int) array->get (array, 0);
  if (value != 4) {
    printf ("FAILED\n");
    return;
  }
  reset ();
  free (stream);
  stream = rivulet_stream_empty ();
  free (stream_last);
  stream_last = stream->last (stream);
  stream_last->add_listener (stream_last, listener);
  length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_periodic () {
  reset ();
  printf ("TEST RivuletStream.periodic()\n");
  RivuletStream *stream_periodic = rivulet_stream_periodic (2);
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
  printf ("TEST RivuletStream.delay()\n");
  RivuletStream *stream_periodic = rivulet_stream_periodic (1);
  RivuletStream *stream_delay = stream_periodic->delay (stream_periodic, 1);
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

static int sample_map_helper = 0;

int sample_map (int value) {
  return (int) (++sample_map_helper % 1024);
}

void test_sample () {
  reset ();
  printf ("TEST RivuletStream.sample()\n");
  RivuletStream *stream_periodic_every_tick = rivulet_stream_periodic (1);
  RivuletStream *stream_periodic_map = stream_periodic_every_tick->map (stream_periodic_every_tick,
                                                                        sample_map);
  RivuletStream *stream_periodic_every_fifty_ticks = rivulet_stream_periodic (50);
  RivuletStream *stream_periodic_map_sampled = stream_periodic_every_fifty_ticks->sample (
          stream_periodic_every_fifty_ticks, stream_periodic_map);
  stream_periodic_map_sampled->add_listener (stream_periodic_map_sampled, listener);
  for (int i = 0; i < 49; i++) {
    rivulet_timer->tick ();
  }
  int length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = array->length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  int value = (int) array->get (array, 0);
  if (value != 50) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < 50; i++) {
    rivulet_timer->tick ();
  }
  length = array->length (array);
  if (length != 2) {
    printf ("FAILED\n");
    return;
  }
  value = (int) array->get (array, 1);
  if (value != 100) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_empty () {
  reset ();
  printf ("TEST RivuletStream.empty()\n");
  RivuletStream *stream = rivulet_stream_empty ();
  stream->add_listener (stream, listener);
  int length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_never () {
  reset ();
  printf ("TEST RivuletStream.never()\n");
  RivuletStream *stream = rivulet_stream_never ();
  stream->add_listener (stream, listener);
  int length = array->length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

typedef struct NewRivuletStream {
  Boolean type;
  RivuletProducerInternal *_producer;
  VariableLengthArray *_internal_listeners;
  RivuletTaskIdentifier _stop_id;
} NewRivuletStream;

void test_memory() {
  printf("size of Boolean: %d bytes\n", (int) sizeof(Boolean));
  printf("size of int: %d bytes\n", (int) sizeof(int));
  printf("size of void *: %d bytes\n", (int) sizeof(void *));
  printf("size of RivuletStream: %d bytes\n", (int) sizeof (RivuletStream));
  printf("size of RivuletProducer: %d bytes\n", (int) sizeof (RivuletProducer));
  printf("size of RivuletListener: %d bytes\n", (int) sizeof (RivuletListener));
  printf("size of NewRivuletStream: %d bytes\n", (int) sizeof(NewRivuletStream));
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
  test_sample ();
  test_empty ();
  test_never ();
  test_memory ();
  return 0;
}
