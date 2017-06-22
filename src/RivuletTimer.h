#ifndef RIVULET_TIMER_H
#define RIVULET_TIMER_H

#include "VariableLengthArray.h"
#include "RivuletTask.h"

typedef Milliseconds (*rivulet_timer_get_milliseconds) ();

typedef struct RivuletTimer {
  VariableLengthArray *_tasks;
  rivulet_timer_get_milliseconds get_milliseconds;
  RivuletTaskIdentifier (*set_timeout) (rivulet_task task, void *argument, int delay);
  RivuletTaskIdentifier (*set_interval) (rivulet_task task, void *argument, int period);
  void (*clear_task) (RivuletTaskIdentifier id);
  void (*tick) ();
} RivuletTimer;

void rivulet_timer_initialize(rivulet_timer_get_milliseconds get_milliseconds);
extern RivuletTimer *rivulet_timer;

#endif //RIVULET_TIMER_H
