#include <Rivulet.h>

RivuletArray *array = NULL;
RivuletListener *listener = NULL;

static Milliseconds _milliseconds = 0;

Milliseconds get_milliseconds () {
  return _milliseconds++;
}

extern void reset_milliseconds () {
  _milliseconds = 0;
}

void _next (RivuletListener *self, int value) {
  rivulet_array_push (array, (void *) (Size) value);
}
void _complete (RivuletListener *self) {

}

void initialize_tests () {
  array = rivulet_array_create ();
  listener = rivulet_listener_create (_next, _complete);
  rivulet_timer_initialize (get_milliseconds);
}

void reset () {
  rivulet_array_clear (array);
  reset_milliseconds ();
  rivulet_array_clear (rivulet_timer->_tasks);
}

int add_one (int value) {
  return value + 1;
}

void test_from_varray () {
  reset ();
  printf ("TEST RivuletStream.fromVariableLengthArray()\n");
  RivuletArray *one_to_four = rivulet_array_create ();
  int a = 1, b = 2, c = 3, d = 4;
  rivulet_array_push (one_to_four, &a);
  rivulet_array_push (one_to_four, &b);
  rivulet_array_push (one_to_four, &c);
  rivulet_array_push (one_to_four, &d);
  RivuletStream *stream = rivulet_stream_from_variable_length_array (one_to_four);
  rivulet_stream_add_listener (stream, listener);
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  rivulet_stream_add_listener (stream, listener);
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_map = rivulet_stream_map (stream, add_one);
  rivulet_stream_add_listener (stream_map, listener);
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_map_to = rivulet_stream_map_to (stream, 10);
  rivulet_stream_add_listener (stream_map_to, listener);
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_filter = rivulet_stream_filter (stream, greater_than_6);
  rivulet_stream_add_listener (stream_filter, listener);
  int length = rivulet_array_length (array);
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_take = rivulet_stream_take (stream, 3);
  rivulet_stream_add_listener (stream_take, listener);
  int length = rivulet_array_length (array);
  if (length != 3) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_drop = rivulet_stream_drop (stream, to_drop);
  rivulet_stream_add_listener (stream_drop, listener);
  int length = rivulet_array_length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
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
  RivuletStream *stream_last = rivulet_stream_last (stream);
  rivulet_stream_add_listener (stream_last, listener);
  int length = rivulet_array_length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  int value = (int) rivulet_array_get (array, 0);
  if (value != 4) {
    printf ("FAILED\n");
    return;
  }
  reset ();
  free (stream);
  stream = rivulet_stream_empty ();
  free (stream_last);
  stream_last = rivulet_stream_last (stream);
  rivulet_stream_add_listener (stream_last, listener);
  length = rivulet_array_length (array);
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
  rivulet_stream_add_listener (stream_periodic, listener);
  int length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
  if (length == 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
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
  RivuletStream *stream_delay = rivulet_stream_delay (stream_periodic, 1);
  rivulet_stream_add_listener (stream_delay, listener);
  int length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
  if (length == 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
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
  RivuletStream *stream_periodic_map = rivulet_stream_map (stream_periodic_every_tick, sample_map);
  RivuletStream *stream_periodic_every_fifty_ticks = rivulet_stream_periodic (50);
  RivuletStream *stream_periodic_map_sampled = rivulet_stream_sample (stream_periodic_every_fifty_ticks, stream_periodic_map);
  rivulet_stream_add_listener (stream_periodic_map_sampled, listener);
  for (int i = 0; i < 49; i++) {
    rivulet_timer->tick ();
  }
  int length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  rivulet_timer->tick ();
  length = rivulet_array_length (array);
  if (length != 1) {
    printf ("FAILED\n");
    return;
  }
  int value = (int) rivulet_array_get (array, 0);
  if (value != 50) {
    printf ("FAILED\n");
    return;
  }
  for (int i = 0; i < 50; i++) {
    rivulet_timer->tick ();
  }
  length = rivulet_array_length (array);
  if (length != 2) {
    printf ("FAILED\n");
    return;
  }
  value = (int) rivulet_array_get (array, 1);
  if (value != 100) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

int sum (int previous, int current) {
  return previous + current;
}

void test_fold () {
  reset ();
  printf ("TEST RivuletStream.fold()\n");
  int five_to_eight[4] = {5, 6, 7, 8};
  RivuletStream *stream = rivulet_stream_from_array (five_to_eight, 4);
  RivuletStream *stream_fold = rivulet_stream_fold (stream, sum, 0);
  rivulet_stream_add_listener (stream_fold, listener);
  int length = rivulet_array_length (array);
  int expected[5] = {0, 5, 11, 18, 26};
  for (int i = 0; i < length; i++) {
    int value = (int) rivulet_array_get (array, i);
    if (value == expected[i]) continue;
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_empty () {
  reset ();
  printf ("TEST RivuletStream.empty()\n");
  RivuletStream *stream = rivulet_stream_empty ();
  rivulet_stream_add_listener (stream, listener);
  int length = rivulet_array_length (array);
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
  rivulet_stream_add_listener (stream, listener);
  int length = rivulet_array_length (array);
  if (length != 0) {
    printf ("FAILED\n");
    return;
  }
  printf ("PASSED\n");
}

void test_memory () {
  printf ("size of Boolean: %d bytes\n", (int) sizeof (Boolean));
  printf ("size of int: %d bytes\n", (int) sizeof (int));
  printf ("size of void *: %d bytes\n", (int) sizeof (void *));
  printf ("size of RivuletArray: %d bytes\n", (int) sizeof (RivuletArray));
  printf ("size of RivuletProducer: %d bytes\n", (int) sizeof (RivuletProducer));
  printf ("size of RivuletProducer with registration: %d bytes\n",
          (int) (sizeof (RivuletProducer) + sizeof (rivulet_producer_start) + sizeof (rivulet_producer_stop) + sizeof (void *)));
  printf ("size of RivuletListener: %d bytes\n", (int) sizeof (RivuletListener));
  printf ("size of RivuletListener with registration: %d bytes\n",
          (int) (sizeof (RivuletListener) + sizeof (rivulet_listener_next) + sizeof (rivulet_listener_complete) + sizeof (void *)));
  printf ("size of RivuletStream: %d bytes\n", (int) sizeof (RivuletStream));
  printf ("size of RivuletOperator: %d bytes\n", (int) sizeof (RivuletOperator));
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
  test_fold ();
  test_empty ();
  test_never ();
  test_memory ();
  return 0;
}
