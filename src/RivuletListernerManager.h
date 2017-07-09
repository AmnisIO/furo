#ifndef RIVULET_LISTERNERMANAGER_H
#define RIVULET_LISTERNERMANAGER_H

#include "RivuletListener.h"

rivulet_listener_internal_next rivulet_listener_internal_next_get (RivuletListenerInternal *listener);
rivulet_listener_internal_complete rivulet_listener_internal_complete_get (RivuletListenerInternal *listener);

#endif //RIVULET_LISTERNERMANAGER_H
