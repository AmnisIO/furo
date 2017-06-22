#include "utils.h"
#include "RivuletTask.h"
#include "RivuletTimer.h"

static int task_id_counter = 0;

RivuletTask *rivulet_task_create (rivulet_task callback, void *argument, RivuletTaskType type, int period) {
  RivuletTask *task = xmalloc (sizeof (RivuletTask));
  task->callback = callback;
  task->argument = argument;
  task->type = type;
  task->_id = task_id_counter++;
  task->period = period;
  task->last_called_at = rivulet_timer->get_milliseconds();
  return task;
}