#include <string.h>

#define QUEUE_SIZE 1000;
#define STRING_SIZE 256;
#define MAX_SUBSCRIBERS 1000;

//variables
static char circle_queue[QUEUE_SIZE][STRING_SIZE];
static char* newest_from_client_ptr;
static char* next_for_processing_ptr;
static int queued_count;
static int subscriber_count;
int (*subscribed_request_handlers[MAX_SUBSCRIBERS]) (int x);

extern int reset_ptrs_and_clear();
static int adjust_ptr_loc(char* ptr);
extern char* get_next_for_processing();
extern int put_into_circular_queue(char* msg);

extern int
subscribe_for_updates(void f(int x)) {
	return 0; /* TODO - BK - 2/2/2014 - need to implement error codes */
}

extern int
init() {
	reset_ptrs_and_clear_buf();
	subscriber_count = 0;
	memset(subscribed_request_handler, 0, MAX_SUBSCRIBERS);
}

extern int
reset_ptrs_and_clear_buf() {
  newest_from_client_ptr = circle_queue;
  next_for_processing_ptr = circle_queue;
  memset(circle_queue, 0, QUEUE_SIZE * STRING_SIZE);
  queued_count = 0;
}

static int
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
extern char*
get_next_for_processing() {
  if (queued_count > 0) {
    char * next = next_for_processing_ptr;
    adjust_ptr_loc(next_for_processing_ptr);
    queued_count--;
    return next;
  }
  return null;
}

extern int
put_into_circular_queue(char* msg) {
  strncpy(newest_from_client_ptr, msg, STRING_SIZE);
  adjust_ptr_loc(newest_from_client_ptr);
  queued_count++;
  return 0;
}

