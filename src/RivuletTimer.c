#include "RivuletTimer.h"

RivuletTimer *rivulet_timer = NULL;

static RivuletTaskIdentifier _set_timeout (rivulet_task callback, void *argument, int delay) {
  RivuletTask *task = rivulet_task_create (callback, argument, RIVULET_TASK_TYPE_SINGLE, delay);
  rivulet_timer->_tasks->push (rivulet_timer->_tasks, task);
  return task->_id;
}

static RivuletTaskIdentifier _set_interval (rivulet_task callback, void *argument, int period) {
  RivuletTask *task = rivulet_task_create (callback, argument, RIVULET_TASK_TYPE_PERIODIC, period);
  rivulet_timer->_tasks->push (rivulet_timer->_tasks, task);
  return task->_id;
}

static Boolean _find_by_id (void *left, void *right) {
  return ((RivuletTask *) left)->_id == ((RivuletTask *) right)->_id;
}

static void _clear_task (RivuletTaskIdentifier id) {
  VariableLengthArray *tasks = rivulet_timer->_tasks;
  RivuletTask *to_find = xmalloc (sizeof (RivuletTask));
  to_find->_id = id;
  int index = tasks->find_index (tasks, (void *) to_find, _find_by_id);
  if (index == -1) return;
  tasks->remove (tasks, index);
}

static void _tick () {
  VariableLengthArray *tasks = rivulet_timer->_tasks;
  int length = tasks->length (tasks);
  if (length == 0) return;
  Milliseconds current_time = rivulet_timer->get_milliseconds ();
  for (int i = 0; i < length; i++) {
    RivuletTask *task = tasks->get (tasks, i);
    if (current_time < (task->last_called_at + task->period)) continue;
    task->last_called_at = current_time;
    task->callback (task->argument);
    if (task->type == RIVULET_TASK_TYPE_PERIODIC) continue;
    tasks->remove (tasks, i);
    i--;
  }
}

void rivulet_timer_initialize (rivulet_timer_get_milliseconds get_milliseconds) {
  if (rivulet_timer != NULL) return;
  rivulet_timer = xmalloc (sizeof (RivuletTimer));
  rivulet_timer->get_milliseconds = get_milliseconds;
  rivulet_timer->set_timeout = _set_timeout;
  rivulet_timer->set_interval = _set_interval;
  rivulet_timer->clear_task = _clear_task;
  rivulet_timer->tick = _tick;
  variable_length_array_initialize (&(rivulet_timer->_tasks));
}