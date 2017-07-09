#ifndef RIVULET_LISTENER_H
#define RIVULET_LISTENER_H

typedef struct RivuletListener {
  RivuletListenerType type;
} RivuletListener;

typedef void (*rivulet_listener_next) (RivuletListener *, int);
typedef void (*rivulet_listener_complete) (RivuletListener *);

#endif //RIVULET_LISTENER_H
