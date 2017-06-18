#ifndef RIVULET_BYTESUBSCRIPTION_H
#define RIVULET_BYTESUBSCRIPTION_H

typedef struct ByteSubscription {
  void (*unsubscribe) (struct ByteSubscription *self);
} ByteSubscription;

#endif //RIVULET_BYTESUBSCRIPTION_H
