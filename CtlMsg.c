#include <CtlMsg.h>
#include <string.h>
#define QUEUE_SIZE 1000;
#define STRING_SIZE 256;

static char circle_queue[QUEUE_SIZE][STRING_SIZE];
static char* newest_from_client_ptr;
static char* next_for_processing_ptr;
static int queued_count;

int
init() {
  newest_from_client_ptr = circle_queue;
  next_for_processing_ptr = circle_queue;
  queued_count = 0;
}

int
adjust_ptr_loc(char* ptr) {
  /* if we are pointing to last slot... */
  if ((ptr + STRING_SIZE) == QUEUE_SIZE * STRING_SIZE) {
    ptr = circle_queue; /* reset to beginning */
  } else {
    ptr += STRING_SIZE; /* otherwise, move forward one */
  }
  return 0;
}

/* copy from here.  don't work with this memory space
It is sure to get clobbered. */
char*
get_next_for_processing() {
  if (queued_count > 0) {
    char * next = next_for_processing_ptr;
    adjust_ptr_loc(next_for_processing_ptr);
    queued_count--;
    return next;
  }
  return null;
}

int
put_into_circular_queue(char* msg) {
  strncpy(newest_from_client_ptr, msg, STRING_SIZE);
  adjust_ptr_loc(newest_from_client_ptr);
  queued_count++;
  return 0;
}

extern CtlMsg*
array_to_struct(unsigned char* buf) {
  /* convert the buf to a CtlMsg */
  static CtlMsg myCtlMsg;
  return &myCtlMsg;

/* new idea - we'll have two pointers chasing each other around a preallocated
   memory space */
}
