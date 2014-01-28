#include <CtlMsg.h>

extern CtlMsg*
array_to_struct(unsigned char* buf) {
  /* convert the buf to a CtlMsg */
  static CtlMsg myCtlMsg;
  return &myCtlMsg;
  /* TODO - BK - 1/28/2014 - how best to free this memory to avoid leaks? */
  /* whoa - what if I pre-allocated enough space for 10,000 of these structs
and had pointers handling their location - kind of like keys to rooms
in a hotel?  As these come in I give them their room key and they hang out
as a queue until some other process comes in to use the value */
/*
}
