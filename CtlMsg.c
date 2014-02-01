#include <CtlMsg.h>

extern CtlMsg*
array_to_struct(unsigned char* buf) {
  /* convert the buf to a CtlMsg */
  static CtlMsg myCtlMsg;
  return &myCtlMsg;

/* new idea - we'll have two pointers chasing each other around a preallocated
   memory space */
}
