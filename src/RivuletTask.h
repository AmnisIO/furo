#ifndef RIVULET_TASK_H
#define RIVULET_TASK_H

typedef enum RivuletTaskType {
  RIVULET_TASK_TYPE_NONE,
  RIVULET_TASK_TYPE_SINGLE,
  RIVULET_TASK_TYPE_PERIODIC
} RivuletTaskType;

typedef unsigned long Milliseconds;
typedef int RivuletTaskIdentifier;
typedef void (*rivulet_task) (void *argument);

typedef struct RivuletTask {
  RivuletTaskIdentifier _id;
  RivuletTaskType type;
  rivulet_task callback;
  void *argument;
  Milliseconds last_called_at;
  int period;
} RivuletTask;

RivuletTask *rivulet_task_create (rivulet_task callback, void *argument, RivuletTaskType type, int period);

#endif //RIVULET_TASK_H
